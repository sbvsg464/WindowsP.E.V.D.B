//lib.h
#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <cstdio>
#include <sddl.h>
#include <psapi.h>
#include <lm.h>
#include <Aclapi.h>
#include <winevt.h>
#include <tlhelp32.h>
#include <accctrl.h>
#include <userenv.h>

#pragma comment(lib, "wevtapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "Kernel32.lib")

#ifndef SE_DEBUG_NAME
#define SE_DEBUG_NAME TEXT("SeDebugPrivilege")
#endif
#ifndef SE_ASSIGNPRIMARYTOKEN_NAME
#define SE_ASSIGNPRIMARYTOKEN_NAME TEXT("SeAssignPrimaryTokenPrivilege")
#endif
#ifndef SE_IMPERSONATE_NAME
#define SE_IMPERSONATE_NAME TEXT("SeImpersonatePrivilege")
#endif

struct LanguagePack {
    std::string welcome;
    std::wstring admin_req_body;
    std::wstring admin_req_title;
    std::wstring admin_success_body;
    std::wstring tips_title;
    std::wstring admin_fail_body;
    std::wstring failed_title;
    std::wstring owner_change_failed;
    std::string take_ownership_success;
    std::string about_text;
    std::string help_text;
    std::string policy_menu;
    std::string unknown_command;
    std::string operation_completed;
    std::string ti_cmd_wait;
    std::wstring user_canceled_elevation;
    std::string se_debug_failed;
    std::string pure_system_not_found;
    std::string pure_system_found_pid;
    std::string open_process_failed;
    std::string open_process_token_failed;
    std::string duplicate_token_failed;
    std::string create_process_failed;
    std::string pure_system_success_pid;
    std::string please_wait;
    std::string already_ti;
    std::string restarting_for_system;
    std::string ti_success_hint;
    std::string system_success_hint;
    std::string whoami_ti_system_hint;
    std::string no_need_repeat_elevation;
    std::string admin_already_enabled;
    std::string enter_admin_password;
    std::string confirm_admin_password;
    std::string password_mismatch;
    std::string enable_admin_success;
    std::string enable_admin_failed;
    std::string enter_folder_path;
    std::string taking_ownership;
    std::string take_ownership_success_full_control;
    std::string grant_permission_failed;
    std::wstring exit_confirm_body;
    std::wstring exit_confirm_title;
    std::string exited_code_0;
    std::wstring process_token_info;
    std::string create_privileged_process;
    std::string env_block_failed;
    std::string invalid_token_path;
    std::string enter_exe_path;
    std::string ti_success_hint_2;
    std::string reg_content;
};

inline const LanguagePack LangCN = {
    .welcome = R"(欢迎!版本:6.0.0 Lite
请选择你想要的提权操作:
1.更改PowerShell执行策略
2.获取以administrator接管文件/文件夹功能
3.获取有trustedinstaller权限的cmd
4.获取有system权限的cmd
5.检查当前程序权限
6.将本程序提权为trustedinstaller
7.强开Administrator账户(支持Windows 10/11 Home)
8.让此账户获取指定文件夹的完全控制权限
9.打印所有特权进程
0.以本程序权限唤醒指定exe
a.关于本程序
e.exit
h.help
)",
    .admin_req_body = L"正在尝试申请Administrator权限",
    .admin_req_title = L"权限不足",
    .admin_success_body = L"申请Administrator权限成功\n请在新弹窗里操作！\n点击这个弹窗的任何部分都将关闭两个窗口！",
    .tips_title = L"提示",
    .admin_fail_body = L"申请Administrator权限失败，请尝试手动给予Administrator权限",
    .failed_title = L"失败",
    .owner_change_failed = L"[-] 更改所有者失败，错误码: ",
    .take_ownership_success = "[+] 操作完成，按任意键返回主菜单...\n现在，右键一个文件，你将会看见一个“管理员接管（Take Ownership）”的选项，点击它即可将该文件的所有权和完全控制权限赋予Administrator组\n",
    .about_text = R"(关于本程序(WindowsPrivilegeEscalationVulnerabilityDisplayBox Version:6.0.0 Lite):
作者:3635177522(QQ号)
鸣谢:
C++(最好的编程语言!)
Visual Studio Code(宇宙最强的平台!)
Google Gemini && Microsoft Github Copilot(最强大的AI!)

编译命令:cmd /c chcp 65001>nul && 【你的g++.exe路径】 -fdiagnostics-color=always -g 【你的main.cpp路径】 -o 【你的生成的exe路径】 -std=c++26 -std=c23 -ladvapi32 -luserenv -lwtsapi32 -Wpsabi -lnetapi32 -lwevtapi -static-libgcc -static-libstdc++
赞助:去我GitHub项目主页点个赞吧~链接:\https://github.com/sbvsg464/WindowsPrivilegeEscalationVulnerabilityDisplayBox
)",
    .help_text = R"(帮助中心(介绍什么时候他们有用):
1.更改PowerShell执行策略: 想执行ps1的时候被拦截，不是你自己代码的问题!
2.获取以administrator接管文件/文件夹功能: 使Administrator是文件夹的拥有者，搭配功能8可以让当前用户获取对文件夹的完全控制权限
3.获取有trustedinstaller权限的cmd: 执行操作被trustedinstaller拦截的时候，比如格式化system32
4.获取有system权限的cmd: 以system权限打开一个命令提示符窗口: 执行操作被system拦截的时候，比如格式化ProgramData
5.检查当前程序权限: 帮助用户检查是否拥有Administrator、trustedinstaller或system权限
6.将本程序提权为trustedinstaller: 将当前程序提升为trustedinstaller权限
7.强开Administrator账户(支持Windows 10/11 Home): 在Windows 7 8 8.x 10 11中Administrator默认禁用状态(不是Administrator权限被禁用，是Administrator这个账户被禁用)
8.让此账户获取指定文件夹的完全控制权限: 赋予此账户对指定文件夹的完全控制权限(如果操作失败，可以搭配功能2使用)
9.打印所有特权进程: 列出当前系统中所有拥有特权令牌的进程及其对应的用户信息
0.以本程序权限唤醒指定exe: 使用当前程序权限启动指定的可执行文件
a.关于本程序: 显示程序相关信息
e.exit: 退出程序
h.help: 显示此帮助信息
)",
    .policy_menu = R"(请选择你想要更改的PowerShell执行策略类型：
1.Restricted(禁止所有 .ps1 脚本)
2.AllSigned(所有脚本都必须数字签名)
3.RemoteSigned(本地脚本可直接运行，网络脚本必须签名)
4.Unrestricted(允许所有脚本运行，但第一次提示)
5.Bypass(不阻止任何脚本运行)
6.check(查看当前powershell策略)
)",
    .unknown_command = "[-] 未知命令\n",
    .operation_completed = "[+] 操作完成，按任意键返回主菜单...\n",
    .ti_cmd_wait = "[+] 正在尝试以trustedinstaller权限弹出cmd.exe，请稍候（弹出窗口后，可输入whoami /groups | findstr Trusted来检测所有者，返回NT SERVICE\\TrustedInstaller即为有trustedinstaller权限）...\n",
    .user_canceled_elevation = L"用户取消了提权请求",
    .se_debug_failed = "[-] 启用 SeDebugPrivilege 失败\n",
    .pure_system_not_found = "[-] 未找到纯 system 进程\n",
    .pure_system_found_pid = "[+] 找到纯净的 system 进程 PID: ",
    .open_process_failed = "[-] OpenProcess 失败: ",
    .open_process_token_failed = "[-] OpenProcessToken 失败: ",
    .duplicate_token_failed = "[-] DuplicateTokenEx 失败: ",
    .create_process_failed = "[-] CreateProcessAsUserW 失败: ",
    .pure_system_success_pid = "[+] 成功创建纯 system 进程，PID: ",
    .please_wait = "[+] 请稍等\n",
    .already_ti = "[*] 当前已经是trustedinstaller权限\n",
    .restarting_for_system = "[*] 正在以trustedinstaller权限重新启动本程序以获取system权限，弹出窗口后再次选择选项4来继续操作...\n",
    .ti_success_hint = "[+]如果启动成功，在新窗口输入whoami /groups | findstr Trusted\n有 NT SERVICE\\TrustedInstaller 行，说明成功获取了trustedinstaller权限\n",
    .system_success_hint = "[+]如果启动成功，在新窗口输入whoami /user\n返回nt authority\\system（或者输入whoami /groups | findstr Trusted无结果），说明成功获取了system权限\n",
    .whoami_ti_system_hint = "[+] trustedinstaller和system都会返回nt authority\\system\n",
    .no_need_repeat_elevation = "无需重复提权，按任意键返回主菜单...\n",
    .admin_already_enabled = "[*] Administrator账户已开启，无需操作，按任意键返回主菜单...\n",
    .enter_admin_password = "请输入要设置的Administrator密码（建议复杂密码）: ",
    .confirm_admin_password = "请再次输入密码以确认: ",
    .password_mismatch = "[-] 两次输入的密码不匹配，操作取消，按任意键返回主菜单...\n",
    .enable_admin_success = "[+] 成功启用Administrator账户并设置密码，按任意键返回主菜单...\n",
    .enable_admin_failed = "[-] 启用Administrator账户失败，按任意键返回主菜单...\n",
    .enter_folder_path = "请输入要恢复并获取完全控制权限的文件夹路径: ",
    .taking_ownership = "[*] 正在尝试夺回文件所有权...\n",
    .take_ownership_success_full_control = "[+] 成功夺回所有权并赋予当前用户完全控制权限！\n",
    .grant_permission_failed = "[-] 赋予权限失败，请检查路径或尝试用选项6\n",
    .exit_confirm_body = L"不要退出好不好，我想一直陪着主人喵",
    .exit_confirm_title = L"要退出了喵!",
    .exited_code_0 = "[+] 已退出，代码:0\n",
    .process_token_info = L"[PID %-5u] %-25ls -> %ls\\%ls\n",
    .create_privileged_process = "[+] 成功创建特权进程，PID: ",
    .env_block_failed = "[-] CreateEnvironmentBlock 失败，尝试无环境变量启动...\n",
    .invalid_token_path = "[-] 无效的令牌句柄或可执行文件路径\n",
    .enter_exe_path = "请输入要执行的程序路径(例如: C:\\Windows\\System32\\cmd.exe): ",
    .ti_success_hint_2 = "如果是以TI权限启动的，输入whoami /groups | findstr Trusted\n有 NT SERVICE\\TrustedInstaller 行，说明成功获取了TI权限\n",
    .reg_content = R"(Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\*\shell\runas]
@="管理员接管（Take Ownership）"

[HKEY_CLASSES_ROOT\*\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"

[HKEY_CLASSES_ROOT\Directory\shell\runas]
@="管理员接管（Take Ownership）"
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\Directory\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
)"
};

inline const LanguagePack LangEN = {
    .welcome = R"(Welcome! Version: 6.0.0 Lite
Please select the privilege escalation operation you want:
1. Change PowerShell Execution Policy
2. Gain Administrator ownership of files/folders
3. Get cmd with TrustedInstaller privileges
4. Get cmd with System privileges
5. Check current program privileges
6. Elevate this program to TrustedInstaller
7. Force enable Administrator account (supports Windows 10/11 Home)
8. Grant this account full control permissions for a specified folder
9. List all privileged processes
0. Launch specified exe with current privileges
a. About this program
e. Exit
h. Help
)",
    .admin_req_body = L"Attempting to request Administrator privileges...",
    .admin_req_title = L"Insufficient Privileges",
    .admin_success_body = L"Successfully requested Administrator privileges.\nPlease operate in the new popup!\nClicking any part of this window will close both windows!",
    .tips_title = L"Information",
    .admin_fail_body = L"Failed to request Administrator privileges. Please try running this program as Administrator manually.",
    .failed_title = L"Error",
    .owner_change_failed = L"[-] Failed to change owner, error code: ",
    .take_ownership_success = "[+] Operation completed, press any key to return to the main menu...\nNow, right-click a file and you will see an option \"Take Ownership (Admin)\", click it to grant the Administrator group full ownership and control permissions of the file.\n",
    .about_text = R"(About This Program (WindowsPrivilegeEscalationVulnerabilityDisplayBox Version:6.0.0 Lite):
Author: 3635177522 (QQ)
Credits:
C++ (The Best Programming Language!)
Visual Studio Code (The Most Powerful IDE!)
Google Gemini && Microsoft Github Copilot(The Most Powerful AI!)

Build Command: cmd /c chcp 65001>nul && [Your g++.exe Path] -fdiagnostics-color=always -g [Your main.cpp Path] -o [Your generated exe Path] -std=c++26 -std=c23 -ladvapi32 -luserenv -lwtsapi32 -Wpsabi -lnetapi32 -lwevtapi -static-libgcc -static-libstdc++
Support: Please star my GitHub project~ Link: https://github.com/sbvsg464/WindowsPrivilegeEscalationVulnerabilityDisplayBox
)",
    .help_text = R"(Help Center (Explains when they are useful):
1. Change PowerShell Execution Policy: Use when blocked from executing .ps1 scripts; it's not an issue with your code!
2. Gain Administrator file/folder ownership: Make Administrator the owner of folders. Use with function 8 to grant the current user full control permissions over the folder.
3. Get cmd with TrustedInstaller privileges: Use when operations are blocked by TrustedInstaller, such as modifying System32.
4. Get cmd with System privileges: Open a command prompt with System privileges. Use when operations are blocked by System, such as modifying ProgramData.
5. Check current program privileges: Helps you check if you currently have Administrator, TrustedInstaller, or System privileges.
6. Elevate this program to TrustedInstaller: Elevate the current program to TrustedInstaller privileges.
7. Force enable Administrator account (Supports Windows 10/11 Home): The Administrator account is disabled by default in Windows 7, 8, 8.x, 10, 11.
8. Grant this account full control of a specified folder: Give this account full control permissions for a specified folder (If it fails, use with function 2).
9. List all privileged processes: List all processes with privileged tokens and their corresponding user information in the current system.
0. Launch specified exe with current privileges: Start a specified executable file with the current program's privileges.
a. About this program: Show program information.
e. exit: Exit the program.
h. help: Show this help message.
)",
    .policy_menu = R"(Please select the PowerShell execution policy type to change:
1. Restricted (Prohibits all .ps1 scripts)
2. AllSigned (All scripts must be digitally signed)
3. RemoteSigned (Local scripts can run directly, network scripts must be signed)
4. Unrestricted (Allows all scripts to run, prompts on first run)
5. Bypass (Does not block any scripts from running)
6. check (View current PowerShell policy)
)",
    .unknown_command = "[-] Unknown command\n",
    .operation_completed = "[+] Operation completed, press any key to return to the main menu...\n",
    .ti_cmd_wait = "[+] Attempting to launch cmd.exe with TrustedInstaller privileges, please wait (After the window pops up, you can enter 'whoami /groups | findstr Trusted' to check the owner. If it returns 'NT SERVICE\\TrustedInstaller', it means you have TrustedInstaller privileges)...\n",
    .user_canceled_elevation = L"User canceled the elevation request",
    .se_debug_failed = "[-] Failed to enable SeDebugPrivilege\n",
    .pure_system_not_found = "[-] Pure System process not found\n",
    .pure_system_found_pid = "[+] Found pure System process PID: ",
    .open_process_failed = "[-] OpenProcess failed: ",
    .open_process_token_failed = "[-] OpenProcessToken failed: ",
    .duplicate_token_failed = "[-] DuplicateTokenEx failed: ",
    .create_process_failed = "[-] CreateProcessAsUserW failed: ",
    .pure_system_success_pid = "[+] Successfully created pure System process, PID: ",
    .please_wait = "[+] Please wait\n",
    .already_ti = "[*] Currently already running with TrustedInstaller privileges\n",
    .restarting_for_system = "[*] Restarting this program with TrustedInstaller privileges to acquire System privileges. Select option 4 again after the new window opens to continue...\n",
    .ti_success_hint = "[+] If started successfully, type 'whoami /groups | findstr Trusted' in the new window.\nIf 'NT SERVICE\\TrustedInstaller' is present, you have successfully acquired TrustedInstaller privileges.\n",
    .system_success_hint = "[+] If started successfully, type 'whoami /user' in the new window.\nIf it returns 'nt authority\\system' (or if 'whoami /groups | findstr Trusted' yields no result), you have successfully acquired System privileges.\n",
    .whoami_ti_system_hint = "[+] Both TrustedInstaller and System will return 'nt authority\\system' for the user.\n",
    .no_need_repeat_elevation = "No need to repeat elevation, press any key to return to the main menu...\n",
    .admin_already_enabled = "[*] Administrator account is already enabled, press any key to return to the main menu...\n",
    .enter_admin_password = "Enter the Administrator password to set (complex password recommended): ",
    .confirm_admin_password = "Enter the password again to confirm: ",
    .password_mismatch = "[-] Passwords do not match, operation canceled. Press any key to return to the main menu...\n",
    .enable_admin_success = "[+] Successfully enabled Administrator account and set password. Press any key to return to the main menu...\n",
    .enable_admin_failed = "[-] Failed to enable Administrator account. Press any key to return to the main menu...\n",
    .enter_folder_path = "Enter the folder path to restore ownership and gain full control permissions: ",
    .taking_ownership = "[*] Attempting to take ownership of files...\n",
    .take_ownership_success_full_control = "[+] Successfully took ownership and granted full control permissions to the current user!\n",
    .grant_permission_failed = "[-] Failed to grant permissions. Check the path or try option 6.\n",
    .exit_confirm_body = L"Please don't exit, I want to stay with Master meow~",
    .exit_confirm_title = L"I'm exiting meow!",
    .exited_code_0 = "[+] Exited, code: 0\n",
    .process_token_info = L"[PID %-5u] %-25ls -> %ls\\%ls\n",
    .create_privileged_process = "[+] Successfully created privileged process, PID: ",
    .env_block_failed = "[-] CreateEnvironmentBlock failed, trying to launch without environment variables...\n",
    .invalid_token_path = "[-] Invalid token handle or executable file path\n",
    .enter_exe_path = "Enter the path of the program to execute (e.g., C:\\Windows\\System32\\cmd.exe): ",
    .ti_success_hint_2 = "If it's launched with TI (TrustedInstaller) privileges, type 'whoami /groups | findstr Trusted' in the new window.\nIf 'NT SERVICE\\TrustedInstaller' is present, you have successfully acquired TI privileges.\n",
    .reg_content = R"REG(Windows Registry Editor Version 5.00

[HKEY_CLASSES_ROOT\*\shell\runas]
@="Take Ownership (Admin)"

[HKEY_CLASSES_ROOT\*\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"

[HKEY_CLASSES_ROOT\Directory\shell\runas]
@="Take Ownership (Admin)"
"NoWorkingDirectory"=""

[HKEY_CLASSES_ROOT\Directory\shell\runas\command]
@="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
"IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
)REG"
};

inline LanguagePack CurrentLang = LangCN;

inline std::string& msg_welcome = CurrentLang.welcome;
inline std::wstring& msg_admin_req_body = CurrentLang.admin_req_body;
inline std::wstring& msg_admin_req_title = CurrentLang.admin_req_title;
inline std::wstring& msg_admin_success_body = CurrentLang.admin_success_body;
inline std::wstring& msg_tips_title = CurrentLang.tips_title;
inline std::wstring& msg_admin_fail_body = CurrentLang.admin_fail_body;
inline std::wstring& msg_failed_title = CurrentLang.failed_title;
inline std::wstring& msg_owner_change_failed = CurrentLang.owner_change_failed;
inline std::string& msg_take_ownership_success = CurrentLang.take_ownership_success;
inline std::string& msg_about_text = CurrentLang.about_text;
inline std::string& msg_help_text = CurrentLang.help_text;
inline std::string& msg_policy_menu = CurrentLang.policy_menu;
inline std::string& msg_unknown_command = CurrentLang.unknown_command;
inline std::string& msg_operation_completed = CurrentLang.operation_completed;
inline std::string& msg_ti_cmd_wait = CurrentLang.ti_cmd_wait;
inline std::wstring& msg_user_canceled_elevation = CurrentLang.user_canceled_elevation;
inline std::string& msg_se_debug_failed = CurrentLang.se_debug_failed;
inline std::string& msg_pure_system_not_found = CurrentLang.pure_system_not_found;
inline std::string& msg_pure_system_found_pid = CurrentLang.pure_system_found_pid;
inline std::string& msg_open_process_failed = CurrentLang.open_process_failed;
inline std::string& msg_open_process_token_failed = CurrentLang.open_process_token_failed;
inline std::string& msg_duplicate_token_failed = CurrentLang.duplicate_token_failed;
inline std::string& msg_create_process_failed = CurrentLang.create_process_failed;
inline std::string& msg_pure_system_success_pid = CurrentLang.pure_system_success_pid;
inline std::string& msg_please_wait = CurrentLang.please_wait;
inline std::string& msg_already_ti = CurrentLang.already_ti;
inline std::string& msg_restarting_for_system = CurrentLang.restarting_for_system;
inline std::string& msg_ti_success_hint = CurrentLang.ti_success_hint;
inline std::string& msg_system_success_hint = CurrentLang.system_success_hint;
inline std::string& msg_whoami_ti_system_hint = CurrentLang.whoami_ti_system_hint;
inline std::string& msg_no_need_repeat_elevation = CurrentLang.no_need_repeat_elevation;
inline std::string& msg_admin_already_enabled = CurrentLang.admin_already_enabled;
inline std::string& msg_enter_admin_password = CurrentLang.enter_admin_password;
inline std::string& msg_confirm_admin_password = CurrentLang.confirm_admin_password;
inline std::string& msg_password_mismatch = CurrentLang.password_mismatch;
inline std::string& msg_enable_admin_success = CurrentLang.enable_admin_success;
inline std::string& msg_enable_admin_failed = CurrentLang.enable_admin_failed;
inline std::string& msg_enter_folder_path = CurrentLang.enter_folder_path;
inline std::string& msg_taking_ownership = CurrentLang.taking_ownership;
inline std::string& msg_take_ownership_success_full_control = CurrentLang.take_ownership_success_full_control;
inline std::string& msg_grant_permission_failed = CurrentLang.grant_permission_failed;
inline std::wstring& msg_exit_confirm_body = CurrentLang.exit_confirm_body;
inline std::wstring& msg_exit_confirm_title = CurrentLang.exit_confirm_title;
inline std::string& msg_exited_code_0 = CurrentLang.exited_code_0;
inline std::wstring& msg_process_token_info = CurrentLang.process_token_info;
inline std::string& msg_create_privileged_process = CurrentLang.create_privileged_process;
inline std::string& msg_env_block_failed = CurrentLang.env_block_failed;
inline std::string& msg_invalid_token_path = CurrentLang.invalid_token_path;
inline std::string& msg_enter_exe_path = CurrentLang.enter_exe_path;
inline std::string& msg_ti_success_hint_2 = CurrentLang.ti_success_hint_2;
inline std::string& msg_reg_content = CurrentLang.reg_content;

inline bool isEnglishSystem() {
    return GetUserDefaultUILanguage() == 0x0409;
}

inline void language() {
    if (isEnglishSystem()) CurrentLang = LangEN;
}

std::wstring ReadPasswordMasked() {
    std::wstring password;
    wchar_t ch;
    while ((ch = _getwch()) != L'\r') {
        if (ch == L'\b') {
            if (!password.empty()) {
                password.pop_back();
                std::wcout << L"\b \b";
            }
        } else {
            password.push_back(ch);
            std::wcout << L"#";
        }
    }
    std::wcout << L"\n";
    return password;
}

void helpCenter() {
    std::system("cls");
    std::cout << msg_help_text;
    std::system("pause");
}

void about() {
    std::system("cls");
    std::cout << msg_about_text;
    std::system("pause");
}