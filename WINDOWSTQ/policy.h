//policy.h
#pragma once

#include "lib.h"

void changePowershellPolicy() {
    std::system("cls");
    std::cout << msg_policy_menu;
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':
            std::system("powershell -Command \"Set-ExecutionPolicy Restricted -Scope LocalMachine\"");
            break;
        case '2':
            std::system("powershell -Command \"Set-ExecutionPolicy AllSigned -Scope LocalMachine\"");
            break;
        case '3':
            std::system("powershell -Command \"Set-ExecutionPolicy RemoteSigned -Scope LocalMachine\"");
            break;
        case '4':
            std::system("powershell -Command \"Set-ExecutionPolicy Unrestricted -Scope LocalMachine\"");
            break;
        case '5':
            std::system("powershell -Command \"Set-ExecutionPolicy Bypass -Scope LocalMachine\"");
            break;
        case '6':
            std::system("powershell -Command \"Get-ExecutionPolicy -List\"");
            break;
        default:
            std::cout << msg_unknown_command;
            break;
    }
    std::cout << msg_operation_completed;
    std::system("pause");
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