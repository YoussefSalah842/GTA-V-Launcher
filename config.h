#ifndef CONFIG_H
#define CONFIG_H

#include <string>

void LoadConfig();
void SaveConfig();
std::string GetGamePath();
bool IsModsEnabled();
void SetGamePath(const std::string& path);
void SetModsEnabled(bool enabled);

#endif // CONFIG_H
