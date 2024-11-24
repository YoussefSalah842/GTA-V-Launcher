#include "config.h"
#include <fstream>
#include <iostream>
#include <string>

static std::string gamePath = "GTAVLauncher.exe";
static bool modsEnabled = true;

void LoadConfig() {
    // For simplicity, just hardcoding here.
    // Normally, you would read from a file (e.g., .ini or .json)
    std::ifstream configFile("config.txt");
    if (configFile.is_open()) {
        std::getline(configFile, gamePath);
        configFile >> modsEnabled;
        configFile.close();
    }
}

void SaveConfig() {
    std::ofstream configFile("config.txt");
    if (configFile.is_open()) {
        configFile << gamePath << std::endl;
        configFile << modsEnabled << std::endl;
        configFile.close();
    }
}

std::string GetGamePath() {
    return gamePath;
}

bool IsModsEnabled() {
    return modsEnabled;
}

void SetGamePath(const std::string& path) {
    gamePath = path;
}

void SetModsEnabled(bool enabled) {
    modsEnabled = enabled;
}
