#include "log.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

void Log(const std::string& level, const std::string& message) {
    std::ofstream logFile("app.log", std::ios_base::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(0);
        char* dt = std::ctime(&now);
        dt[strlen(dt) - 1] = '\0'; // Remove trailing newline
        logFile << "[" << dt << "] [" << level << "] " << message << std::endl;
        logFile.close();
    }
}

void LogInfo(const char* message) {
    Log("INFO", message);
}

void LogWarning(const char* message) {
    Log("WARNING", message);
}

void LogError(const char* message) {
    Log("ERROR", message);
}
