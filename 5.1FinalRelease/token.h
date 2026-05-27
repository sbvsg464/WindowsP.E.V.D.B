// token.h
#pragma once

#include "lib.h"

bool IsRunAsAdmin() {//检测是否为管理员
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0,
        &adminGroup)) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

bool IsTrustedInstaller() {//检测是否为trustedinstaller权限
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        return false;
    DWORD len = 0;
    GetTokenInformation(hToken, TokenGroups, NULL, 0, &len);
    std::vector<BYTE> buffer(len);
    bool result = false;
    if (GetTokenInformation(hToken, TokenGroups, buffer.data(), len, &len)) {
        PTOKEN_GROUPS pGroups = (PTOKEN_GROUPS)buffer.data();
        for (DWORD i = 0; i < pGroups->GroupCount; i++) {
            LPSTR sidStr = NULL;
            if (ConvertSidToStringSidA(pGroups->Groups[i].Sid, &sidStr)) {
                if (strncmp(sidStr, "S-1-5-80-956008885", 18) == 0) {
                    result = true;
                    LocalFree(sidStr);
                    break;
                }
                LocalFree(sidStr);
            }
        }
    }
    CloseHandle(hToken);
    return result;
}

bool IsAdministratorDisabled() {
    LPUSER_INFO_4 info = nullptr;
    if (NetUserGetInfo(nullptr, L"Administrator", 4, (LPBYTE*)&info) != NERR_Success)
        return true;
    bool disabled = info->usri4_flags & UF_ACCOUNTDISABLE;
    NetApiBufferFree(info);
    return disabled;
}

bool IsSystem() {//检测是否为system权限
    HANDLE hToken = NULL;
    DWORD dwLength = 0;
    PTOKEN_USER pTokenUser = NULL;
    bool result = false;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        return false;
    }
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLength);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        CloseHandle(hToken);
        return false;
    }
    pTokenUser = (PTOKEN_USER)LocalAlloc(LPTR, dwLength);
    if (!pTokenUser) {
        CloseHandle(hToken);
        return false;
    }
    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwLength, &dwLength)) {
        if (IsWellKnownSid(pTokenUser->User.Sid, WinLocalSystemSid)) {
            result = true;
        }
    }
    LocalFree(pTokenUser);
    CloseHandle(hToken);
    return result;
}


bool EnableAdministrator() {
    USER_INFO_1008 info{};
    info.usri1008_flags = UF_SCRIPT; // 不包含 UF_ACCOUNTDISABLE
    DWORD err = 0;
    NET_API_STATUS status = NetUserSetInfo(
        nullptr,
        L"Administrator",
        1008,
        (LPBYTE)&info,
        &err
    );
    return status == NERR_Success;
};

PSID GetCurrentUserSid() {
    HANDLE token = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        return nullptr;
    }
    DWORD size = 0;
    GetTokenInformation(token, TokenUser, nullptr, 0, &size);
    if (size == 0) {
        CloseHandle(token);
        return nullptr;
    }
    PTOKEN_USER user = (PTOKEN_USER)malloc(size);
    if (!user) {
        CloseHandle(token);
        return nullptr;
    }
    PSID resultSid = nullptr;
    if (GetTokenInformation(token, TokenUser, user, size, &size)) {
        if (IsValidSid(user->User.Sid)) {
            DWORD sidLength = GetLengthSid(user->User.Sid);
            resultSid = (PSID)LocalAlloc(LMEM_FIXED, sidLength);
            if (resultSid) {
                if (!CopySid(sidLength, resultSid, user->User.Sid)) {
                    LocalFree(resultSid);
                    resultSid = nullptr;
                }
            }
        }
    }
    free(user);
    CloseHandle(token);
    return resultSid; 
}

bool RestoreOwnerToAdministrators(const std::wstring& path) {
    PSID adminSid = nullptr;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    bool success = false;
    if (!AllocateAndInitializeSid(&ntAuthority, 2, 
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0, &adminSid)) {
        return false;
    }
    DWORD res = SetNamedSecurityInfoW(
        const_cast<LPWSTR>(path.c_str()),
        SE_FILE_OBJECT,
        OWNER_SECURITY_INFORMATION, // 明确指定修改所有者
        adminSid,                   // 将新所有者设为管理员组
        nullptr,
        nullptr,
        nullptr
    );
    if (res == ERROR_SUCCESS) {
        success = true;
    } else {
        std::wcerr << L"[-] 更改所有者失败，错误码: " << res << std::endl;
    }
    if (adminSid) FreeSid(adminSid);
    return success;
}