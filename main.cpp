#include <windows.h>
#include <shellapi.h>
#include <direct.h>
#include <sys/stat.h>
#include <errno.h>

void LaunchGame(const char* gamePath, bool runAsAdmin, bool withoutMods) {
    if (withoutMods) {
        MoveFile("mods", "mods_disabled");
    }

    SHELLEXECUTEINFO sei = { 0 };
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = runAsAdmin ? "runas" : "open";
    sei.lpFile = gamePath;
    sei.nShow = SW_SHOWNORMAL;
    sei.hwnd = NULL;

    if (!ShellExecuteEx(&sei)) {
        MessageBox(NULL, "Failed to launch the game.", "Error", MB_ICONERROR | MB_OK);
    }

    if (sei.hProcess) {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }

    if (withoutMods) {
        MoveFile("mods_disabled", "mods");
    }
}

void CreateModsFolder() {
    if (_mkdir("mods") == 0) {
        MessageBox(NULL, "The 'mods' folder was successfully created.", "Info", MB_ICONINFORMATION | MB_OK);
    } else if (errno == EEXIST) {
        MessageBox(NULL, "The 'mods' folder already exists.", "Info", MB_ICONINFORMATION | MB_OK);
    } else {
        MessageBox(NULL, strerror(errno), "Error Creating Folder", MB_ICONERROR | MB_OK);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            LaunchGame("GTAVLauncher.exe", false, false);
            break;
        case 2:
            LaunchGame("GTAVLauncher.exe", true, false);
            break;
        case 3:
            LaunchGame("GTAVLauncher.exe", false, true);
            break;
        case 4:
            CreateModsFolder();
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* CLASS_NAME = "GTA Launcher";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "GTA V Launcher",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 250,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    CreateWindow("STATIC", "GTA V Launcher", WS_VISIBLE | WS_CHILD | SS_CENTER,
                 50, 10, 200, 20, hwnd, NULL, hInstance, NULL);

    CreateWindow("BUTTON", "Run Normally", WS_VISIBLE | WS_CHILD,
                 50, 30, 200, 30, hwnd, (HMENU)1, hInstance, NULL);

    CreateWindow("BUTTON", "Run as Admin", WS_VISIBLE | WS_CHILD,
                 50, 70, 200, 30, hwnd, (HMENU)2, hInstance, NULL);

    CreateWindow("BUTTON", "Run Without Mods", WS_VISIBLE | WS_CHILD,
                 50, 110, 200, 30, hwnd, (HMENU)3, hInstance, NULL);

    CreateWindow("BUTTON", "Create Mods Folder", WS_VISIBLE | WS_CHILD,
                 50, 150, 200, 30, hwnd, (HMENU)4, hInstance, NULL);

    ShowWindow(hwnd, SW_NORMAL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

