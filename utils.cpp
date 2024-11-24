#include "utils.h"
#include <sys/stat.h>
#include <windows.h>
#include <stdio.h>

bool FileExists(const char* path) {
    DWORD fileAttrib = GetFileAttributes(path);
    return (fileAttrib != INVALID_FILE_ATTRIBUTES && !(fileAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool CreateDirectorySafe(const char* path) {
    if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    return false;
}
