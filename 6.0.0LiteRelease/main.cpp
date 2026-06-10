//main.cpp
#include "menu.h"

int main() {
    std::system("chcp 65001 > nul");
    language();
    if (!CheckNativeArchitecture()) {
        std::wstring finalErrorMsg = msg_arch_mismatch_body + ExpectedArch
                                      + ExpectedArchStr + 
                                     CurrentArch + CurrentArchStr;
                                     
        MessageBoxW(nullptr, finalErrorMsg.c_str(), msg_arch_mismatch_title.c_str(), MB_ICONERROR | MB_OK);
        return 1;
    }
    if (!IsRunAsAdmin()) {
        MessageBoxW(nullptr, msg_admin_req_body.c_str(), msg_admin_req_title.c_str(), MB_ICONINFORMATION | MB_OK);
        if (elevateProcess()) {
            MessageBoxW(nullptr, msg_admin_success_body.c_str(), msg_tips_title.c_str(), MB_ICONINFORMATION | MB_OK);
            return 0;
        } else {
            MessageBoxW(nullptr, msg_admin_fail_body.c_str(), msg_failed_title.c_str(), MB_ICONERROR | MB_OK);
            return 1;
        }
    }
    ShowMainMenu();
    return 0;
}