// menu.h
#pragma once

#include "lib.h"
#include "acl.h"
#include "system.h"
#include "token.h"
#include "policy.h"
#include "else.h"

void ShowMainMenu() {
here:
    std::system("cls");
    std::cout << msg_welcome;
    char option;
    std::cin >> option;
    switch (option) {
        case '1':
            changePowershellPolicy();
            goto here;
        case '2':
            WriteRegFile();
            goto here;
        case '3':
            privilegeEscalationForTI();
            std::cout << msg_ti_success_hint;
            std::system("pause");
            goto here;
        case '4':
            beforeRunAssystem();
            std::cout << msg_system_success_hint;
            std::system("pause");
            goto here;
        case '5':
            std::system("whoami");
            std::system("whoami /groups | findstr Trusted");
            std::cout << msg_whoami_ti_system_hint;
            std::system("pause");
            goto here;
        case '6':
            std::system("cls");
            {
                if (IsTrustedInstaller()) {
                    std::cout << msg_already_ti;
                    std::cout << msg_no_need_repeat_elevation;
                    std::system("pause");
                    goto here;
                }
                std::cout << msg_please_wait;
                WCHAR currentPathW[MAX_PATH];
                GetModuleFileNameW(NULL, currentPathW, MAX_PATH);
                std::wstring currentPathStr = L"\"" + std::wstring(currentPathW) + L"\"";
                std::cout << msg_Elevate_privileges_to_TI_tips;
                if (!RunAsTrustedInstaller(currentPathStr)) {
                    std::cerr << msg_Elevate_privileges_to_TI_tips2;
                    char currentPathA[MAX_PATH];
                    GetModuleFileNameA(NULL, currentPathA, MAX_PATH);
                    std::string psCmd = "powershell -NoProfile -ExecutionPolicy Bypass -Command \""
                        "Install-Module -Name NtObjectManager -Force -Scope CurrentUser -ErrorAction SilentlyContinue; "
                        "Import-Module NtObjectManager; "
                        "sc.exe start TrustedInstaller; "
                        "Set-NtTokenPrivilege SeDebugPrivilege; "
                        "$p = Get-NtProcess -Name TrustedInstaller.exe; "
                        "New-Win32Process '" + std::string(currentPathA) + "' -CreationFlags NewConsole -ParentProcess $p"
                        "\"";
                    std::system(psCmd.c_str());
                }
                std::system("pause");
                goto here;
            }
        case '7':
            {
                if (!IsAdministratorDisabled()) {
                    std::cout << msg_admin_already_enabled;
                    std::system("pause");
                    goto here;
                }
                std::cout << msg_enter_admin_password;
                std::wstring password = ReadPasswordMasked();
                std::cout << msg_confirm_admin_password;
                std::wstring confirmPassword = ReadPasswordMasked();
                if (password != confirmPassword) {
                    std::cout << msg_password_mismatch;
                    std::system("pause");
                    goto here;
                }
                if (SetAdministratorPassword(password) && EnableAdministrator()) {
                    std::cout << msg_enable_admin_success;
                } 
                else {
                    std::cout << msg_enable_admin_failed;
                }
                std::system("pause");
                goto here;
            }
        case '8':
        {
            std::system("cls");
            std::cout << msg_enter_folder_path;
            std::wstring folderPath;
            std::wcin >> folderPath;
            std::cout << msg_taking_ownership;
            RestoreOwnerToAdministrators(folderPath); 
            if (GrantFullControlToCurrentUser(folderPath)) {
                std::cout << msg_take_ownership_success_full_control;
            } else {
                std::cout << msg_grant_permission_failed;
            }
            std::system("pause");
            goto here;
        }
        case '9':
            EnumProcessTokens();
            std::system("pause");
            goto here;
        case '0':
            {
                HANDLE hToken = Custom_GetCurrentPrimaryToken();
                if (!hToken) {
                    std::cout << msg_invalid_token_path;
                    std::system("pause");
                    goto here;
                }
                std::wstring pach;
                std::cout << msg_enter_exe_path;
                std::wcin >> pach;
                LaunchProcessWithToken(hToken, pach);
                std::cout << msg_ti_success_hint_2;
            }
            std::system("pause");
            goto here;
        case 'e':
            {
                int ret = MessageBoxW(nullptr, msg_exit_confirm_body.c_str(), msg_exit_confirm_title.c_str(), MB_ICONINFORMATION | MB_OKCANCEL);
                if (ret == IDOK) {
                    break;
                }
                std::cout << msg_exited_code_0;
                goto here;
            }
        case 'h':
            helpCenter();
            goto here;
        case 'a':
            about();
            goto here;
        default:
            std::cout << msg_unknown_command;
            std::system("pause");
            goto here;
    }
    return;
}