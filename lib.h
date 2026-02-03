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
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <sddl.h>
#include <psapi.h>
#include <lm.h>
#include <Aclapi.h>
#include <winevt.h>
#include <tlhelp32.h>

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
    system("cls");
    std::cout << "帮助中心(介绍什么时候他们有用):\n"
    "1.更改PowerShell执行策略: 想执行ps1的时候被拦截，不是你自己代码的问题!\n"
    "2.获取以administrator接管文件/文件夹功能: 使Administrator是文件夹的拥有者，搭配功能8可以让当前用户获取对文件夹的完全控制权限\n"
    "3.获取有trustedinstaller权限的cmd: 执行操作被trustedinstaller拦截的时候，比如格式化System32\n"
    "4.获取有SYSTEM权限的cmd: 以SYSTEM权限打开一个命令提示符窗口: 执行操作被SYSTEM拦截的时候，比如格式化ProgramData\n"
    "5.检查当前程序权限: 帮助用户检查是否拥有Administrator、trustedinstaller或SYSTEM权限\n"
    "6.将本程序提权为trustedinstaller: 将当前程序提升为trustedinstaller权限\n"
    "7.强开Administrator账户(支持Windows 10/11 Home): 在Windows 7 8 8.x 10 11中Administrator默认禁用状态(不是Administrator权限被禁用，是Administrator这个账户被禁用)\n"
    "8.让此账户获取指定文件夹的完全控制权限: 赋予此账户对指定文件夹的完全控制权限(如果操作失败，可以搭配功能2使用)\n"
    "9.打印所有特权进程: 列出当前系统中所有拥有特权令牌的进程及其对应的用户信息\n"
    "e.exit: 退出程序\n"
    "h.help: 显示此帮助信息\n";
    std::cout << "[+] 操作完成，按任意键返回主菜单...\n";
    system("pause");
}
