#include <windows.h>
#include <string>
#include <iostream>
#include <commdlg.h>

std::string getGamePath() {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Executable Files\0*.exe\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Select GTAVLauncher.exe";
    if (GetOpenFileName(&ofn)) {
        return std::string(fileName);
    }
    return "";
}

void launchGame(const std::string& argument) {
    std::string gamePath = getGamePath();
    if (gamePath.empty()) {
        MessageBoxW(NULL, L"Game path selection canceled.", L"Info", MB_ICONINFORMATION);
        return;
    }
    DWORD fileAttr = GetFileAttributesA(gamePath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        MessageBoxW(NULL, L"GTAVLauncher.exe not found at the selected path!", L"Error", MB_ICONERROR);
        return;
    }
    std::string command = gamePath + " " + argument;
    std::wstring wideCommand(command.begin(), command.end());
    STARTUPINFOW si = {0};
    PROCESS_INFORMATION pi = {0};
    if (CreateProcessW(NULL, const_cast<wchar_t*>(wideCommand.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        MessageBoxW(NULL, L"Game launched successfully!", L"Info", MB_ICONINFORMATION);
    } else {
        MessageBoxW(NULL, L"Failed to launch the game.", L"Error", MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                launchGame("-safeMode");
            } else if (LOWORD(wParam) == 2) {
                launchGame("-fullscreen");
            } else if (LOWORD(wParam) == 3) {
                launchGame("-windowed");
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

HWND createButton(HWND parent, const char* text, int x, int y, int width, int height, int id, HINSTANCE hInstance) {
    return CreateWindow("BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                        x, y, width, height, parent, (HMENU)id, hInstance, NULL);
}

int main() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "GTALauncherWindow";
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return -1;
    }
    HWND hwnd = CreateWindow(wc.lpszClassName, "Select the Type of Client", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, wc.hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
        return -1;
    }
    createButton(hwnd, "-safeMode", 100, 30, 200, 30, 1, wc.hInstance);
    createButton(hwnd, "-fullscreen", 100, 70, 200, 30, 2, wc.hInstance);
    createButton(hwnd, "-windowed", 100, 110, 200, 30, 3, wc.hInstance);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
