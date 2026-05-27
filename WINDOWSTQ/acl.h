// acl.h
#pragma once

#include "lib.h"
#include "token.h"

bool GrantFullControlToCurrentUser(const std::wstring& path) {
    PSID userSid = GetCurrentUserSid();
    if (!userSid) return false;
    PACL oldDacl = nullptr;
    PACL newDacl = nullptr;
    PSECURITY_DESCRIPTOR sd = nullptr;
    bool success = false;
    DWORD res = GetNamedSecurityInfoW(
        path.c_str(),
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        nullptr,
        nullptr,
        &oldDacl,
        nullptr,
        &sd
    );
    if (res != ERROR_SUCCESS) {
        if (sd) LocalFree(sd);
        if (newDacl) LocalFree(newDacl);
        if (userSid) LocalFree(userSid); 
        return success;
    }
    EXPLICIT_ACCESSW ea{};
    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfAccessMode = GRANT_ACCESS;
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = (LPWSTR)userSid;
    res = SetEntriesInAclW(1, &ea, oldDacl, &newDacl);
    if (res != ERROR_SUCCESS) {
        if (sd) LocalFree(sd);
        if (newDacl) LocalFree(newDacl);
        if (userSid) LocalFree(userSid); 
        return success;
    }
    res = SetNamedSecurityInfoW(
        const_cast<LPWSTR>(path.c_str()), 
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        nullptr,
        nullptr,
        newDacl,
        nullptr
    );
    if (res == ERROR_SUCCESS) {
        success = true;
    }
    return success;
}

void WriteRegFile() {
    std::system("cls");
    std::ofstream reg("ti.reg", std::ios::binary);
    reg << msg_reg_content;
    reg.close();
    std::system("reg import ti.reg");
    std::filesystem::remove("ti.reg");
    std::cout << msg_take_ownership_success;
    std::system("pause");
}