
#include "mod/MyMod.h"

#include <filesystem>

#include <pl/Mod.hpp>

namespace clange_me {

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
}

ClangeMeMod::ClangeMeMod()
    : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    auto &self = getSelf();
    self.getLogger().debug("Loading DoubleDoor...");

    std::error_code ec;

    std::filesystem::create_directories(
        self.getDataDir(), ec
    );

    if (ec) {
        self.getLogger().error(
            "Failed to create data directory {}: {}",
            self.getDataDir().string(),
            ec.message()
        );
        return false;
    }

    std::filesystem::create_directories(
        self.getConfigDir(), ec
    );

    if (ec) {
        self.getLogger().error(
            "Failed to create config directory {}: {}",
            self.getConfigDir().string(),
            ec.message()
        );
        return false;
    }

    mConfigFile.emplace();

    if (!mConfigFile->load()) {
        self.getLogger().warn("Failed to load typed config");
        return false;
    }

    mConfig = mConfigFile->value();

    self.getLogger().info(
        "DoubleDoor loaded from {}",
        self.getModDir().string()
    );

    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().debug("Enabling DoubleDoor...");

    if (!mConfig.enabled) {
        self.getLogger().info("DoubleDoor is disabled by config");
        return true;
    }

    self.getLogger().warn(
        "DoubleDoor: interaction hook is not installed yet."
    );

    return true;
}

bool ClangeMeMod::disable() {
    auto &self = getSelf();
    self.getLogger().debug("Disabling DoubleDoor...");

    if (mDoorHook) {
        mDoorHook->reset();
        mDoorHook.reset();
    }

    return true;
}

bool ClangeMeMod::unload() {
    auto &self = getSelf();
    self.getLogger().debug("Unloading DoubleDoor...");

    if (mDoorHook) {
        mDoorHook->reset();
        mDoorHook.reset();
    }

    mConfigFile.reset();

    return true;
}

} // namespace clange_me
