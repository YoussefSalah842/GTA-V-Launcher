#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

// Helper function to check if a file exists
bool FileExists(const char* path);

// Function to create directories with error handling
bool CreateDirectorySafe(const char* path);

#endif // UTILS_H
