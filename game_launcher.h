#ifndef GAME_LAUNCHER_H
#define GAME_LAUNCHER_H

#include <windows.h>

void LogMessage(const char* message);
void RestoreModsIfDisabled();
void LaunchGame(const char* gamePath, bool runAsAdmin, bool withoutMods);
void BrowseForGamePath(char* gamePathBuffer, size_t bufferSize);
void CreateModsFolder();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id, HINSTANCE hInstance);
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif // GAME_LAUNCHER_H
