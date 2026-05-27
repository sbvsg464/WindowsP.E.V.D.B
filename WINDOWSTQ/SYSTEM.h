// system.h
#pragma once

#include "lib.h"
#include "token.h"

void privilegeEscalationForTI() {
    std::system("cls");
    std::cout << msg_ti_cmd_wait;
    std::system("powershell -NoProfile -ExecutionPolicy Bypass -Command \"Install-Module -Name NtObjectManager -Force -Scope CurrentUser; Import-Module NtObjectManager; sc.exe start TrustedInstaller; Set-NtTokenPrivilege SeDebugPrivilege; $p = Get-NtProcess -Name TrustedInstaller.exe; New-Win32Process cmd.exe -CreationFlags NewConsole -ParentProcess $p\"");
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
    std::wstring cmd = L"cmd.exe /k \"whoami /user && echo 检查是否有TI组:&whoami /groups | findstr Trusted && echo [如果有TI则说明不是std::system]&pause\"";
    BOOL success = CreateProcessAsUserW(
        hDupToken, 
        NULL, 
        &cmd[0], 
        NULL, NULL, 
        FALSE,
        CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, 
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
    if (IsTrustedInstaller()) {
        std::cout << msg_already_ti;
        RunAsPureSystem();
        return;
    } else {
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