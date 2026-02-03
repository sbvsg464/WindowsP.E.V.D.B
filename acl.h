// acl.h
#pragma once

#include "lib.h"
#include "token.h"

bool GrantFullControlToCurrentUser(const std::wstring& path) {//授予完全控制权限
    PSID userSid = GetCurrentUserSid();
    if (!userSid) return false;
    EXPLICIT_ACCESSW ea{};
    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfAccessMode = GRANT_ACCESS;
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = (LPWSTR)userSid;
    PACL oldDacl = nullptr;
    PACL newDacl = nullptr;
    PSECURITY_DESCRIPTOR sd = nullptr;
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
    if (res != ERROR_SUCCESS)
        return false;
    res = SetEntriesInAclW(1, &ea, oldDacl, &newDacl);
    if (res != ERROR_SUCCESS)
        return false;
    res = SetNamedSecurityInfoW(
        (LPWSTR)path.c_str(),
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        nullptr,
        nullptr,
        newDacl,
        nullptr
    );
    if (sd) LocalFree(sd);
    if (newDacl) LocalFree(newDacl);
    return res == ERROR_SUCCESS;
}

void WriteRegFile() {//写入注册表文件以实现右键接管文件功能
    system("cls");
    std::ofstream reg("ti.reg", std::ios::binary);//这个就是内容
    reg <<
    R"(Windows Registry Editor Version 5.00

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
    ;
    reg.close();
    system("reg import ti.reg");
    std::filesystem::remove("ti.reg");
    std::cout << "[+] 操作完成，按任意键返回主菜单...\n现在，右键一个文件，你将会看见一个“管理员接管（Take Ownership）”的选项，点击它即可将该文件的所有权和完全控制权限赋予administrator组\n";
    system("pause");
}
