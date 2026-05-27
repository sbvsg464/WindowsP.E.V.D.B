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

std::string msg_welcome = "欢迎!版本:6.0.0 Lite\n请选择你想要的提权操作:\n"
    "1.更改PowerShell执行策略\n2.获取以administrator接管文件/文件夹功能\n3.获取有trustedinstaller权限的cmd\n"
    "4.获取有system权限的cmd\n5.检查当前程序权限\n6.将本程序提权为trustedinstaller\n7.强开Administrator账户(支持Windows 10/11 Home)\n"
    "8.让此账户获取指定文件夹的完全控制权限\n9.打印所有特权进程\na.关于本程序\ne.exit\nh.help\n";
std::wstring msg_admin_req_body = L"正在尝试申请Administrator权限";
std::wstring msg_admin_req_title = L"权限不足";
std::wstring msg_admin_success_body = L"申请Administrator权限成功\n请在新弹窗里操作！\n点击这个弹窗的任何部分都将关闭两个窗口！";
std::wstring msg_tips_title = L"提示";
std::wstring msg_admin_fail_body = L"申请Administrator权限失败，请尝试手动给予Administrator权限";
std::wstring msg_failed_title = L"失败";
std::wstring msg_owner_change_failed = L"[-] 更改所有者失败，错误码: ";
std::string msg_take_ownership_success = "[+] 操作完成，按任意键返回主菜单...\n现在，右键一个文件，你将会看见一个“管理员接管（Take Ownership）”的选项，点击它即可将该文件的所有权和完全控制权限赋予administrator组\n";
std::string msg_about_text = "关于本程序(WindowsPrivilegeEscalationVulnerabilityDisplayBox Version:6.0.0 Lite):\n"
    "作者:3635177522(QQ号)\n鸣谢:\nC++(最好的编程语言!)\nVisual Studio Code(宇宙最强的平台!)\n"
    "Google Gemini && Microsoft Github Copilot(最强大的AI!)\n\n"
    "编译命令:cmd /c chcp 65001>nul && 【你的g++.exe路径】 -fdiagnostics-color=always -g 【源码路径】 -o 【生成的exe路径】-O3 -std=c++20 -ladvapi32 -luserenv -lwtsapi32 -Wpsabi -lnetapi32 -lwevtapi\n"
    "赞助:去我GitHub项目主页点个赞吧~链接:\\https://github.com/sbvsg464/WindowsPrivilegeEscalationVulnerabilityDisplayBox\n";
std::string msg_help_text = "帮助中心(介绍什么时候他们有用):\n"
    "1.更改PowerShell执行策略: 想执行ps1的时候被拦截，不是你自己代码的问题!\n"
    "2.获取以administrator接管文件/文件夹功能: 使Administrator是文件夹的拥有者，搭配功能8可以让当前用户获取对文件夹的完全控制权限\n"
    "3.获取有trustedinstaller权限的cmd: 执行操作被trustedinstaller拦截的时候，比如格式化system32\n"
    "4.获取有system权限的cmd: 以system权限打开一个命令提示符窗口: 执行操作被system拦截的时候，比如格式化ProgramData\n"
    "5.检查当前程序权限: 帮助用户检查是否拥有Administrator、trustedinstaller或system权限\n"
    "6.将本程序提权为trustedinstaller: 将当前程序提升为trustedinstaller权限\n"
    "7.强开Administrator账户(支持Windows 10/11 Home): 在Windows 7 8 8.x 10 11中Administrator默认禁用状态(不是Administrator权限被禁用，是Administrator这个账户被禁用)\n"
    "8.让此账户获取指定文件夹的完全控制权限: 赋予此账户对指定文件夹的完全控制权限(如果操作失败，可以搭配功能2使用)\n"
    "9.打印所有特权进程: 列出当前系统中所有拥有特权令牌的进程及其对应的用户信息\n"
    "a.关于本程序: 显示程序相关信息\n"
    "e.exit: 退出程序\n"
    "h.help: 显示此帮助信息\n";
std::string msg_policy_menu = "请选择你想要更改的PowerShell执行策略类型：\n1.Restricted(禁止所有 .ps1 脚本)\n2.AllSigned(所有脚本都必须数字签名)\n3.RemoteSigned(本地脚本可直接运行，网络脚本必须签名)\n4.Unrestricted(允许所有脚本运行，但第一次提示)\n5.Bypass(不阻止任何脚本运行)\n6.check(查看当前powershell策略)\n";
std::string msg_unknown_command = "[-] 未知命令\n";
std::string msg_operation_completed = "[+] 操作完成，按任意键返回主菜单...\n";
std::string msg_ti_cmd_wait = "[+] 正在尝试以trustedinstaller权限弹出cmd.exe，请稍候（弹出窗口后，可输入whoami /groups | findstr Trusted来检测所有者，返回NT SERVICE\\TrustedInstaller即为有trustedinstaller权限）...\n";
std::wstring msg_user_canceled_elevation = L"用户取消了提权请求";
std::string msg_se_debug_failed = "[-] 启用 SeDebugPrivilege 失败\n";
std::string msg_pure_system_not_found = "[-] 未找到纯 system 进程\n";
std::string msg_pure_system_found_pid = "[+] 找到纯净的 system 进程 PID: ";
std::string msg_open_process_failed = "[-] OpenProcess 失败: ";
std::string msg_open_process_token_failed = "[-] OpenProcessToken 失败: ";
std::string msg_duplicate_token_failed = "[-] DuplicateTokenEx 失败: ";
std::string msg_create_process_failed = "[-] CreateProcessAsUserW 失败: ";
std::string msg_pure_system_success_pid = "[+] 成功创建纯 system 进程，PID: ";
std::string msg_please_wait = "[+] 请稍等\n";
std::string msg_already_ti = "[*] 当前已经是trustedinstaller权限\n";
std::string msg_restarting_for_system = "[*] 正在以trustedinstaller权限重新启动本程序以获取system权限，弹出窗口后再次选择选项4来继续操作...\n";
std::string msg_ti_success_hint = "[+]如果启动成功，在新窗口输入whoami /groups | findstr Trusted\n有 NT SERVICE\\TrustedInstaller 行，说明成功获取了trustedinstaller权限\n";
std::string msg_system_success_hint = "[+]如果启动成功，在新窗口输入whoami /user\n返回nt authority\\system（或者输入whoami /groups | findstr Trusted无结果），说明成功获取了system权限\n";
std::string msg_whoami_ti_system_hint = "[+] trustedinstaller和system都会返回nt authority\\system\n";
std::string msg_no_need_repeat_elevation = "无需重复提权，按任意键返回主菜单...\n";
std::string msg_admin_already_enabled = "[*] Administrator账户已开启，无需操作，按任意键返回主菜单...\n";
std::string msg_enter_admin_password = "请输入要设置的Administrator密码（建议复杂密码）: ";
std::string msg_confirm_admin_password = "请再次输入密码以确认: ";
std::string msg_password_mismatch = "[-] 两次输入的密码不匹配，操作取消，按任意键返回主菜单...\n";
std::string msg_enable_admin_success = "[+] 成功启用Administrator账户并设置密码，按任意键返回主菜单...\n";
std::string msg_enable_admin_failed = "[-] 启用Administrator账户失败，按任意键返回主菜单...\n";
std::string msg_enter_folder_path = "请输入要恢复并获取完全控制权限的文件夹路径: ";
std::string msg_taking_ownership = "[*] 正在尝试夺回文件所有权...\n";
std::string msg_take_ownership_success_full_control = "[+] 成功夺回所有权并赋予当前用户完全控制权限！\n";
std::string msg_grant_permission_failed = "[-] 赋予权限失败，请检查路径或尝试用选项6\n";
std::wstring msg_exit_confirm_body = L"不要退出好不好，我想一直陪着主人喵";
std::wstring msg_exit_confirm_title = L"要退出了喵!";
std::string msg_exited_code_0 = "[+] 已退出，代码:0\n";
std::wstring msg_process_token_info = L"[PID %-5u] %-25ls -> %ls\\%ls\n";

std::string msg_reg_content = R"(Windows Registry Editor Version 5.00

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
    )";

inline bool isChinesesystem() {
    LANGID lang = GetUserDefaultUILanguage();
    return lang == 0x0804 || lang == 0x0404;
}

inline bool isEnglishsystem() {
    return GetUserDefaultUILanguage() == 0x0409;
}

inline void language() {
    if (isEnglishsystem()) {
        msg_welcome = "Welcome! Version: 6.0.0 Lite\nPlease select the privilege escalation operation you want:\n"
        "1. Change PowerShell Execution Policy\n2. Gain administrator ownership of files/folders\n3. Get cmd with TrustedInstaller privileges\n"
        "4. Get cmd with system privileges\n5. Check current program privileges\n6. Elevate this program to TrustedInstaller\n7. Force enable Administrator account (supports Windows 10/11 Home)\n"
        "8. Grant this account full control permissions for a specified folder\n9. List all privileged processes\na. About this program\ne. Exit\nh. Help\n";
        msg_admin_req_body = L"Attempting to request Administrator privileges";
        msg_admin_req_title = L"Insufficient privileges";
        msg_admin_success_body = L"Successfully requested Administrator privileges\nPlease operate in the new popup!\nClicking any part of this window will close both windows!";
        msg_tips_title = L"Tip";
        msg_admin_fail_body = L"Failed to request Administrator privileges, please try granting Administrator privileges manually";
        msg_failed_title = L"Failed";
        msg_owner_change_failed = L"[-] Failed to change owner, error code: ";
        msg_take_ownership_success = "[+] Operation completed, press any key to return to the main menu...\nNow, right-click a file and you will see an option \"Take Ownership (Admin)\", click it to grant the administrator group full ownership and control permissions of the file\n";
        msg_about_text = "About This Program (WindowsPrivilegeEscalationVulnerabilityDisplayBox Version:6.0.0 Lite):\n"
        "Author: 3635177522 (QQ)\nCredits:\nC++ (The Best Programming Language!)\nVisual Studio Code (The Most Powerful IDE!)\n"
        "Google Gemini && Microsoft Github Copilot(The Most Powerful AI!)\n\n"
        "Build Command: cmd /c chcp 65001>nul && [Your g++.exe Path] -fdiagnostics-color=always -g [Source Code Path] -o [Output Exe Path] -O3 -std=c++20 -ladvapi32 -luserenv -lwtsapi32 -Wpsabi -lnetapi32 -lwevtapi\n"
        "Support: Please star my GitHub project~ Link: https://github.com/sbvsg464/WindowsPrivilegeEscalationVulnerabilityDisplayBox\n";
        msg_help_text = "Help Center (Explains when they are useful):\n"
        "1. Change PowerShell Execution Policy: Blocked when executing PS1 scripts; it's not an issue with your code!\n"
        "2. Gain administrator file/folder ownership: Make Administrator the owner of folders. Use with function 8 to grant the current user full control permissions over the folder.\n"
        "3. Get cmd with TrustedInstaller privileges: Use when operations are blocked by TrustedInstaller, such as modifying std::system32.\n"
        "4. Get cmd with system privileges: Open a command prompt with system privileges. Use when operations are blocked by system, such as modifying ProgramData.\n"
        "5. Check current program privileges: Helps the user check if they have Administrator, TrustedInstaller, or system privileges.\n"
        "6. Elevate this program to TrustedInstaller: Elevate the current program to TrustedInstaller privileges.\n"
        "7. Force enable Administrator account (Supports Windows 10/11 Home): The Administrator account is disabled by default in Windows 7, 8, 8.x, 10, 11.\n"
        "8. Grant this account full control of a specified folder: Give this account full control permissions for a specified folder (If it fails, use with function 2).\n"
        "9. List all privileged processes: List all processes with privileged tokens and their corresponding user information in the current system.\n"
        "a. About this program: Show program information.\n"
        "e. exit: Exit the program.\n"
        "h. help: Show this help message.\n";
        msg_policy_menu = "Please select the PowerShell execution policy type to change:\n1.Restricted (Prohibits all .ps1 scripts)\n2.AllSigned (All scripts must be digitally signed)\n3.RemoteSigned (Local scripts can run directly, network scripts must be signed)\n4.Unrestricted (Allows all scripts to run, prompts on first run)\n5.Bypass (Does not block any scripts from running)\n6.check (View current powershell policy)\n";
        msg_unknown_command = "[-] Unknown command\n";
        msg_operation_completed = "[+] Operation completed, press any key to return to the main menu...\n";
        msg_ti_cmd_wait = "[+] Attempting to pop cmd.exe with TrustedInstaller privileges, please wait...\n";
        msg_user_canceled_elevation = L"User canceled the elevation request";
        msg_se_debug_failed = "[-] Failed to enable SeDebugPrivilege\n";
        msg_pure_system_not_found = "[-] Pure system process not found\n";
        msg_pure_system_found_pid = "[+] Found pure system process PID: ";
        msg_open_process_failed = "[-] OpenProcess failed: ";
        msg_open_process_token_failed = "[-] OpenProcessToken failed: ";
        msg_duplicate_token_failed = "[-] DuplicateTokenEx failed: ";
        msg_create_process_failed = "[-] CreateProcessAsUserW failed: ";
        msg_pure_system_success_pid = "[+] Successfully created pure system process, PID: ";
        msg_please_wait = "[+] Please wait\n";
        msg_already_ti = "[*] Currently already has TrustedInstaller privileges\n";
        msg_restarting_for_system = "[*] Restarting this program with TrustedInstaller privileges to acquire system privileges, select option 4 again to continue...\n";
        msg_ti_success_hint = "[+] If started successfully, type whoami /groups | findstr Trusted in the new window\nIf NT SERVICE\\TrustedInstaller is present, you have successfully acquired TrustedInstaller privileges\n";
        msg_system_success_hint = "[+] If started successfully, type whoami /user in the new window\nIf it returns nt authority\\system, you have successfully acquired system privileges\n";
        msg_whoami_ti_system_hint = "[+] Both TrustedInstaller and system will return nt authority\\system\n";
        msg_no_need_repeat_elevation = "No need to repeat elevation, press any key to return to main menu...\n";
        msg_admin_already_enabled = "[*] Administrator account is already enabled, press any key to return to main menu...\n";
        msg_enter_admin_password = "Enter the Administrator password to set (complex password recommended): ";
        msg_confirm_admin_password = "Enter the password again to confirm: ";
        msg_password_mismatch = "[-] Passwords do not match, operation canceled, press any key to return to main menu...\n";
        msg_enable_admin_success = "[+] Successfully enabled Administrator account and set password, press any key to return to main menu...\n";
        msg_enable_admin_failed = "[-] Failed to enable Administrator account, press any key to return to main menu...\n";
        msg_enter_folder_path = "Enter the folder path to restore and gain full control permissions: ";
        msg_taking_ownership = "[*] Attempting to take ownership of files...\n";
        msg_take_ownership_success_full_control = "[+] Successfully took ownership and granted full control permissions to current user!\n";
        msg_grant_permission_failed = "[-] Failed to grant permissions, check the path or try option 6\n";
        msg_exit_confirm_body = L"Please don't exit, I want to stay with my master meow";
        msg_exit_confirm_title = L"Exiting meow!";
        msg_exited_code_0 = "[+] Exited, code: 0\n";
        msg_process_token_info = L"[PID %-5u] %-25ls -> %ls\\%ls\n";

        msg_reg_content = R"(Windows Registry Editor Version 5.00

    [HKEY_CLASSES_ROOT\*\shell\runas]
    @="Take Ownership （Admin）"

    [HKEY_CLASSES_ROOT\*\shell\runas\command]
    @="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"
    "IsolatedCommand"="cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F"

    [HKEY_CLASSES_ROOT\Directory\shell\runas]
    @="Take Ownership （Admin）"
    "NoWorkingDirectory"=""

    [HKEY_CLASSES_ROOT\Directory\shell\runas\command]
    @="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
    "IsolatedCommand"="cmd.exe /c takeown /f \"%1\" /r /d y && icacls \"%1\" /grant administrators:F /t"
    )";
    }
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