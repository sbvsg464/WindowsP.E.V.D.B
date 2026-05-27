//else.h
#pragma once

#include "lib.h"

void EnumProcessTokens() {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    if (!Process32FirstW(hSnap, &pe)) {
        CloseHandle(hSnap);
        return;
    }
    do {
        HANDLE hProc = OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION,
            FALSE,
            pe.th32ProcessID
        );
        if (!hProc)
            continue;
        HANDLE hToken = nullptr;
        if (OpenProcessToken(hProc, TOKEN_QUERY, &hToken)) {
            DWORD len = 0;
            GetTokenInformation(hToken, TokenUser, nullptr, 0, &len);
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                PTOKEN_USER pUser = (PTOKEN_USER)malloc(len);
                if (GetTokenInformation(hToken, TokenUser, pUser, len, &len)) {
                    WCHAR name[256], domain[256];
                    DWORD nameLen = 256, domainLen = 256;
                    SID_NAME_USE use;

                    if (LookupAccountSidW(
                        nullptr,
                        pUser->User.Sid,
                        name,
                        &nameLen,
                        domain,
                        &domainLen,
                        &use
                    )) {
                        wprintf(L"[PID %-5u] %-25ls -> %ls\\%ls\n",
                        pe.th32ProcessID,
                        pe.szExeFile,
                        domain,
                        name);
                    }
                }
                free(pUser);
            }
            CloseHandle(hToken);
        }
        CloseHandle(hProc);
    } while (Process32NextW(hSnap, &pe));
    CloseHandle(hSnap);
}
