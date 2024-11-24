#include "gtest/gtest.h"
#include "game_launcher.h"

TEST(GameLauncherTests, TestLaunchGame) {
    // Assuming "gamePath" is a valid path to a game executable
    bool result = LaunchGame("GTAVLauncher.exe", false, false);
    EXPECT_TRUE(result);
}

TEST(GameLauncherTests, TestModsFolderCreation) {
    bool result = CreateModsFolder();
    EXPECT_TRUE(result);
}
