//main.cpp
#include "menu.h"

/*将stdlib.h stdio.h改为C++风格的cstdlib cstdio 支持英语 
删减lib.h中多余字符串 提权至trustedinstaller SYSTEM权限添加请等待提示*/
int main() {
    std::system("chcp 65001 > nul");
    language();
    if (!IsRunAsAdmin()) {
        MessageBoxW(nullptr, msg_admin_req_body.c_str(), msg_admin_req_title.c_str(), MB_ICONINFORMATION | MB_OK);
        if (elevateProcess()) {
            MessageBoxW(nullptr, msg_admin_success_body.c_str(), msg_tips_title.c_str(), MB_ICONINFORMATION | MB_OK);
            return 0;
        }
        else {
            MessageBoxW(nullptr, msg_admin_fail_body.c_str(), msg_failed_title.c_str(), MB_ICONERROR | MB_OK);
            return 1;
        }
    }
    ShowMainMenu();
    return 0;
}