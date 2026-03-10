//policy.h
#pragma once

#include "lib.h"

void changePowershellPolicy() {//更改powershell执行策略
    system("cls");
    std::cout << "请选择你想要更改的PowerShell执行策略类型：\n1.Restricted(禁止所有 .ps1 脚本)\n2.AllSigned(所有脚本都必须数字签名)\n3.RemoteSigned(本地脚本可直接运行，网络脚本必须签名)\n4.Unrestricted(允许所有脚本运行，但第一次提示)\n5.Bypass(不阻止任何脚本运行)\n6.check(查看当前powershell策略)\n";
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':
            system("powershell -Command \"Set-ExecutionPolicy Restricted -Scope LocalMachine\"");
            break;
        case '2':
            system("powershell -Command \"Set-ExecutionPolicy AllSigned -Scope LocalMachine\"");
            break;
        case '3':
            system("powershell -Command \"Set-ExecutionPolicy RemoteSigned -Scope LocalMachine\"");
            break;
        case '4':
            system("powershell -Command \"Set-ExecutionPolicy Unrestricted -Scope LocalMachine\"");
            break;
        case '5':
            system("powershell -Command \"Set-ExecutionPolicy Bypass -Scope LocalMachine\"");
            break;
        case '6':
            system("powershell -Command \"Get-ExecutionPolicy -List\"");
            break;
        default:
            std::cout << "未知命令\n";
            break;
    }
    std::cout << "[+] 操作完成，按任意键返回主菜单...\n";
    system("pause");
}

bool SetAdministratorPassword(const std::wstring& password) {
    USER_INFO_1003 info{};
    info.usri1003_password = const_cast<LPWSTR>(password.c_str());
    DWORD err = 0;
    NET_API_STATUS status = NetUserSetInfo(
        nullptr,
        L"Administrator",
        1003,
        (LPBYTE)&info,
        &err
    );
    return status == NERR_Success;
}
