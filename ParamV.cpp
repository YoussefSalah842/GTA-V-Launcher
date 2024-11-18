#include <windows.h>
#include <string>
#include <iostream>

void launchGame(const std::string& argument) {
    // Get the current working directory (where the program is located)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    // Get the directory of the executable manually (without using <filesystem>)
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string gamePath = std::string(buffer).substr(0, pos) + "\\GTAVLauncher.exe";

    // Check if the file exists
    DWORD fileAttr = GetFileAttributesA(gamePath.c_str());
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        MessageBoxW(NULL, L"GTAVLauncher.exe not found!", L"Error", MB_ICONERROR);
        return;
    }

    // Create the command to launch the game with the argument
    std::string command = gamePath + " " + argument;

    // Convert to wide string for CreateProcess
    std::wstring wideCommand(command.begin(), command.end());

    // Setup process information
    STARTUPINFOW si = {0};
    PROCESS_INFORMATION pi = {0};

    // Create the process
    if (CreateProcessW(NULL, const_cast<wchar_t*>(wideCommand.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // Optionally, wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        // If CreateProcess failed, show an error message
        MessageBoxW(NULL, L"Failed to launch the game.", L"Error", MB_ICONERROR);
    }
}

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // First button: -safeMode
                launchGame("-safeMode");
            } else if (LOWORD(wParam) == 2) { // Second button: -fullscreen
                launchGame("-fullscreen");
            } else if (LOWORD(wParam) == 3) { // Third button: -windowed
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

int main() {
    // Define window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc; // Window procedure function
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "GTALauncherWindow";

    // Register window class
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return -1;
    }

    // Create window
    HWND hwnd = CreateWindow(wc.lpszClassName, "Select the Type of Client", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
        return -1;
    }

    // Create buttons
    CreateWindow("BUTTON", "-safeMode", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 100, 30, 200, 30, hwnd, (HMENU)1, wc.hInstance, NULL);
    CreateWindow("BUTTON", "-fullscreen", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 100, 70, 200, 30, hwnd, (HMENU)2, wc.hInstance, NULL);
    CreateWindow("BUTTON", "-windowed", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 100, 110, 200, 30, hwnd, (HMENU)3, wc.hInstance, NULL);

    // Display window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

