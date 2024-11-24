#include <windows.h>
#include <shellapi.h>
#include <direct.h>
#include <sys/stat.h>
#include <errno.h>
#include <commdlg.h>
#include <stdio.h>

void LogMessage(const char* message) {
    FILE* logFile = fopen("game_launcher.log", "a");
    if (logFile) {
        fprintf(logFile, "%s\n", message);
        fclose(logFile);
    }
}

void RestoreModsIfDisabled() {
    if (!MoveFile("mods_disabled", "mods")) {
        LogMessage("Failed to restore mods folder.");
    }
}

void LaunchGame(const char* gamePath, bool runAsAdmin, bool withoutMods) {
    if (withoutMods) {
        if (!MoveFile("mods", "mods_disabled")) {
            DWORD dwError = GetLastError();
            char errorMessage[256];
            sprintf(errorMessage, "Failed to disable mods folder. Error code: %lu", dwError);
            MessageBox(NULL, errorMessage, "Error", MB_ICONERROR | MB_OK);
            return;
        }
    }

    SHELLEXECUTEINFO sei = { 0 };
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = runAsAdmin ? "runas" : "open";
    sei.lpFile = gamePath;
    sei.nShow = SW_SHOWNORMAL;
    sei.hwnd = NULL;

    if (!ShellExecuteEx(&sei)) {
        LogMessage("Game launch failed.");
        MessageBox(NULL, "Failed to launch the game.", "Error", MB_ICONERROR | MB_OK);
        if (withoutMods) RestoreModsIfDisabled();
        return;
    }

    LogMessage("Game launched successfully.");
    if (sei.hProcess) {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }

    if (withoutMods) RestoreModsIfDisabled();
}

void BrowseForGamePath(char* gamePathBuffer, size_t bufferSize) {
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Executables\0*.exe\0All Files\0*.*\0";
    ofn.lpstrFile = gamePathBuffer;
    ofn.nMaxFile = bufferSize;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        strncpy(gamePathBuffer, ofn.lpstrFile, bufferSize);
        gamePathBuffer[bufferSize - 1] = '\0';  // Ensure null-termination
        MessageBox(NULL, "Game path selected successfully.", "Info", MB_ICONINFORMATION | MB_OK);
    } else {
        MessageBox(NULL, "No file selected.", "Warning", MB_ICONWARNING | MB_OK);
    }
}

void CreateModsFolder() {
    if (_mkdir("mods") == 0) {
        MessageBox(NULL, "The 'mods' folder was successfully created.", "Info", MB_ICONINFORMATION | MB_OK);
    } else if (errno == EEXIST) {
        MessageBox(NULL, "The 'mods' folder already exists.", "Info", MB_ICONINFORMATION | MB_OK);
    } else {
        char errorMessage[256];
        snprintf(errorMessage, sizeof(errorMessage), "Error creating folder: %s", strerror(errno));
        MessageBox(NULL, errorMessage, "Error Creating Folder", MB_ICONERROR | MB_OK);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static char gamePath[256] = "GTAVLauncher.exe";

    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: LaunchGame(gamePath, false, false); break;
        case 2: LaunchGame(gamePath, true, false); break;
        case 3: LaunchGame(gamePath, false, true); break;
        case 4: CreateModsFolder(); break;
        case 5: BrowseForGamePath(gamePath, sizeof(gamePath)); break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id, HINSTANCE hInstance) {
    CreateWindow("BUTTON", text, WS_VISIBLE | WS_CHILD,
                 x, y, width, height, hwnd, (HMENU)id, hInstance, NULL);
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
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    CreateButton(hwnd, "Run Normally", 50, 30, 200, 30, 1, hInstance);
    CreateButton(hwnd, "Run as Admin", 50, 70, 200, 30, 2, hInstance);
    CreateButton(hwnd, "Run Without Mods", 50, 110, 200, 30, 3, hInstance);
    CreateButton(hwnd, "Create Mods Folder", 50, 150, 200, 30, 4, hInstance);
    CreateButton(hwnd, "Select Game Path", 50, 190, 200, 30, 5, hInstance);

    ShowWindow(hwnd, SW_NORMAL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
