// system.h
#pragma once

#include "lib.h"
#include "token.h"

bool EnablePrivilege(LPCTSTR privilegeName) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    if (!LookupPrivilegeValue(NULL, privilegeName, &luid)) {
        CloseHandle(hToken);
        return false;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    BOOL result = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    CloseHandle(hToken);
    return result && (GetLastError() == ERROR_SUCCESS);
}

bool IsPuresystemProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) return false;
    HANDLE hToken = NULL;
    bool isPuresystem = false;
    if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        DWORD len = 0;
        GetTokenInformation(hToken, TokenUser, NULL, 0, &len);
        std::vector<BYTE> buffer(len);
        if (GetTokenInformation(hToken, TokenUser, buffer.data(), len, &len)) {
            PTOKEN_USER pUser = (PTOKEN_USER)buffer.data();
            if (IsWellKnownSid(pUser->User.Sid, WinLocalSystemSid)) {
                GetTokenInformation(hToken, TokenGroups, NULL, 0, &len);
                buffer.resize(len);
                if (GetTokenInformation(hToken, TokenGroups, buffer.data(), len, &len)) {
                    PTOKEN_GROUPS pGroups = (PTOKEN_GROUPS)buffer.data();
                    bool hasTI = false;
                    for (DWORD i = 0; i < pGroups->GroupCount; i++) {
                        LPSTR sidStr = NULL;
                        if (ConvertSidToStringSidA(pGroups->Groups[i].Sid, &sidStr)) {
                            if (strncmp(sidStr, "S-1-5-80-", 9) == 0) {
                                hasTI = true;
                                LocalFree(sidStr);
                                break;
                            }
                            LocalFree(sidStr);
                        }
                    }
                    isPuresystem = !hasTI;
                }
            }
        }
        CloseHandle(hToken);
    }
    CloseHandle(hProcess);
    return isPuresystem;
}

DWORD FindPuresystemProcess() {
    DWORD processes[1024], cbNeeded;
    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded))
        return 0;
    for (unsigned i = 0; i < cbNeeded / sizeof(DWORD); i++) {
        if (processes[i] == 0) continue;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
        if (!hProcess) continue;
        WCHAR szProcessName[MAX_PATH] = L"<unknown>";
        HMODULE hMod;
        DWORD cbNeededMod;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
            GetModuleBaseNameW(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(WCHAR));
            if (_wcsicmp(szProcessName, L"services.exe") == 0) {
                if (IsPuresystemProcess(processes[i])) {
                    CloseHandle(hProcess);
                    return processes[i];
                }
            }
            else if ((_wcsicmp(szProcessName, L"wininit.exe") == 0 ||
                     _wcsicmp(szProcessName, L"winlogon.exe") == 0) && 
                     IsPuresystemProcess(processes[i])) {
                CloseHandle(hProcess);
                return processes[i];
            }
        }
        CloseHandle(hProcess);
    }
    return 0;
}

bool RunAsTrustedInstaller(const std::wstring& customCmd = L"") {
    if (!EnablePrivilege(SE_DEBUG_NAME)) {
        std::cerr << msg_se_debug_failed;
        return false;
    }
    EnablePrivilege(SE_IMPERSONATE_NAME);
    EnablePrivilege(SE_ASSIGNPRIMARYTOKEN_NAME);
    DWORD sysPid = FindPuresystemProcess();
    if (sysPid != 0) {
        HANDLE hSysProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, sysPid);
        if (hSysProc) {
            HANDLE hSysToken = NULL;
            if (OpenProcessToken(hSysProc, TOKEN_DUPLICATE | TOKEN_QUERY, &hSysToken)) {
                HANDLE hImpToken = NULL;
                if (DuplicateToken(hSysToken, SecurityImpersonation, &hImpToken)) {
                    ImpersonateLoggedOnUser(hImpToken); 
                    CloseHandle(hImpToken);
                }
                CloseHandle(hSysToken);
            }
            CloseHandle(hSysProc);
        }
    }
    std::cout << msg_LElevate_privileges_to_TI_tips;
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
    if (hSCManager) {
        SC_HANDLE hService = OpenServiceW(hSCManager, L"TrustedInstaller", SERVICE_START | SERVICE_QUERY_STATUS);
        if (hService) {
            SERVICE_STATUS_PROCESS ssp;
            DWORD bytesNeeded;
            QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssp, sizeof(ssp), &bytesNeeded);
            if (ssp.dwCurrentState != SERVICE_RUNNING) {
                StartServiceW(hService, 0, NULL);
                Sleep(1500);
            }
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCManager);
    }
    DWORD pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(hSnap, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, L"TrustedInstaller.exe") == 0) {
                    pid = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnap, &pe));
        }
        CloseHandle(hSnap);
    }
    if (pid == 0) {
        std::cerr << msg_LElevate_privileges_to_TI_tips2;
        RevertToSelf(); 
        return false;
    }
    std::cout << msg_LElevate_privileges_to_TI_tips3 << pid << "\n";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) {
        std::cerr << msg_open_process_failed << GetLastError() << "\n";
        RevertToSelf(); 
        return false;
    }
    HANDLE hToken = NULL;
    if (!OpenProcessToken(hProcess, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &hToken)) {
        std::cerr << msg_open_process_token_failed << GetLastError() << "\n";
        CloseHandle(hProcess);
        RevertToSelf(); 
        return false;
    }
    RevertToSelf(); 
    HANDLE hDupToken = NULL;
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hDupToken)) {
        std::cerr << msg_duplicate_token_failed << GetLastError() << "\n";
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return false;
    }
    DWORD sessionId = WTSGetActiveConsoleSessionId();
    if (sessionId != 0xFFFFFFFF) {
        SetTokenInformation(hDupToken, TokenSessionId, &sessionId, sizeof(sessionId));
    }
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    std::wstring cmd = customCmd.empty() ? msg_command_check_TI : customCmd;
    BOOL success = CreateProcessWithTokenW(
        hDupToken, 
        0,  
        NULL, 
        &cmd[0], 
        CREATE_NEW_CONSOLE, 
        NULL, 
        NULL, 
        &si, 
        &pi
    );
    if (!success) {
        std::cerr << msg_create_process_failed << GetLastError() << "\n";
    } else {
        std::cout << msg_LElevate_privileges_to_TI_tips4 << pi.dwProcessId << "\n";
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    CloseHandle(hDupToken);
    CloseHandle(hToken);
    CloseHandle(hProcess);
    return success;
}

void privilegeEscalationForTI() {
    std::system("cls");
    std::cout << msg_ti_cmd_wait;
    std::cout << msg_Elevate_privileges_to_TI_tips;
    if (!RunAsTrustedInstaller()) {
        std::cerr << msg_Elevate_privileges_to_TI_tips2;
        std::system("powershell -NoProfile -ExecutionPolicy Bypass -Command \"Install-Module -Name NtObjectManager -Force -Scope CurrentUser -ErrorAction SilentlyContinue; Import-Module NtObjectManager; sc.exe start TrustedInstaller; Set-NtTokenPrivilege SeDebugPrivilege; $p = Get-NtProcess -Name TrustedInstaller.exe; New-Win32Process cmd.exe -CreationFlags NewConsole -ParentProcess $p\"");
    }
    std::cout << msg_operation_completed;
    std::system("pause");
}

bool elevateProcess() {
    WCHAR exePath[MAX_PATH];
    if (!GetModuleFileNameW(NULL, exePath, MAX_PATH)) {
        return false;
    }
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = exePath;
    sei.lpParameters = GetCommandLineW();
    sei.nShow = SW_NORMAL;
    sei.fMask = SEE_MASK_NO_CONSOLE;
    if (!ShellExecuteExW(&sei)) {
        DWORD error = GetLastError();
        if (error == ERROR_CANCELLED) {
            MessageBoxW(nullptr, msg_user_canceled_elevation.c_str(), msg_tips_title.c_str(), MB_ICONWARNING | MB_OK);
        }
        return false;
    }
    return true;
}

bool RunAsPureSystem() {
    std::system("pause");
    if (!EnablePrivilege(SE_DEBUG_NAME)) {
        std::cerr << msg_se_debug_failed;
        return false;
    }
    EnablePrivilege(SE_DEBUG_NAME);
    EnablePrivilege(SE_IMPERSONATE_NAME);
    EnablePrivilege(SE_ASSIGNPRIMARYTOKEN_NAME);
    DWORD pid = FindPuresystemProcess();
    if (pid == 0) {
        std::cerr << msg_pure_system_not_found;
        return false;
    }
    std::cout << msg_pure_system_found_pid << pid << "\n";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) {
        std::cerr << msg_open_process_failed << GetLastError() << "\n";
        return false;
    }
    HANDLE hToken = NULL;
    if (!OpenProcessToken(hProcess, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &hToken)) {
        std::cerr << msg_open_process_token_failed << GetLastError() << "\n";
        CloseHandle(hProcess);
        return false;
    }
    HANDLE hDupToken = NULL;
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hDupToken)) {
        std::cerr << msg_duplicate_token_failed << GetLastError() << "\n";
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return false;
    }
    DWORD sessionId = WTSGetActiveConsoleSessionId();
    if (sessionId != 0xFFFFFFFF) {
        SetTokenInformation(hDupToken, TokenSessionId, &sessionId, sizeof(sessionId));
    }
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    std::wstring cmd = msg_command_check_SYSTEM;
    BOOL success = CreateProcessWithTokenW(
        hDupToken, 
        0,  
        NULL, 
        &cmd[0], 
        CREATE_NEW_CONSOLE, 
        NULL, 
        NULL, 
        &si, 
        &pi
    );
    if (!success) {
        std::cerr << msg_create_process_failed << GetLastError() << "\n";
    } else {
        std::cout << msg_pure_system_success_pid << pi.dwProcessId << "\n";
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    CloseHandle(hDupToken);
    CloseHandle(hToken);
    CloseHandle(hProcess);
    return success;
}

void beforeRunAssystem() {
    std::system("cls");
    std::cout << msg_please_wait;
    std::cout << msg_Elevate_privileges_to_SYSTEM_tips;
    if (RunAsPureSystem()) {
        return; 
    } else {
        std::cerr << msg_Elevate_privileges_to_SYSTEM_tips2;  
        if (IsTrustedInstaller()) {
            RunAsPureSystem();
            return;
        }
        char currentPath[MAX_PATH];
        GetModuleFileNameA(NULL, currentPath, MAX_PATH);
        std::string psCmd = "powershell -NoProfile -ExecutionPolicy Bypass -Command \""
            "Install-Module -Name NtObjectManager -Force -Scope CurrentUser -ErrorAction SilentlyContinue; "
            "Import-Module NtObjectManager; "
            "sc.exe start TrustedInstaller; "
            "Set-NtTokenPrivilege SeDebugPrivilege; "
            "$p = Get-NtProcess -Name TrustedInstaller.exe; "
            "New-Win32Process '" + std::string(currentPath) + "' -CreationFlags NewConsole -ParentProcess $p"
            "\"";
        std::cout << msg_restarting_for_system;
        std::system(psCmd.c_str());
    }
}

bool LaunchProcessWithToken(HANDLE hPrimaryToken, const std::wstring& exePath) {
    if (hPrimaryToken == NULL || exePath.empty()) {
        std::cout << msg_invalid_token_path;
        return false;
    }
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    std::wstring desktopName = L"winsta0\\default";
    si.lpDesktop = &desktopName[0];
    std::wstring cmdLine = exePath;
    LPVOID pEnvBlock = NULL;
    if (!CreateEnvironmentBlock(&pEnvBlock, hPrimaryToken, FALSE)) {
        std::cout << msg_env_block_failed;
    }
    BOOL bResult = CreateProcessWithTokenW(
        hPrimaryToken,
        0,  
        NULL,
        &cmdLine[0],
        CREATE_NEW_CONSOLE | (pEnvBlock ? CREATE_UNICODE_ENVIRONMENT : 0), 
        pEnvBlock,
        NULL, 
        &si,
        &pi
    );
    if (bResult) {
        std::cout << msg_create_privileged_process << pi.dwProcessId << std::endl;
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    } else {
        std::cout << msg_create_process_failed << GetLastError() << std::endl;
    }
    if (pEnvBlock) {
        DestroyEnvironmentBlock(pEnvBlock);
    }
    return bResult != FALSE;
}