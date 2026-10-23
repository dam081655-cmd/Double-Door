#include "mod/MyMod.h"

#include <filesystem>

#include <pl/Mod.hpp>
#include <pl/memory/Hook.hpp>
#include <pl/memory/Signature.hpp>

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
            "Failed to create data directory: {}",
            ec.message()
        );
        return false;
    }

    std::filesystem::create_directories(
        self.getConfigDir(), ec
    );

    if (ec) {
        self.getLogger().error(
            "Failed to create config directory: {}",
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

    self.getLogger().info("DoubleDoor loaded");
    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();
    self.getLogger().debug("Enabling DoubleDoor...");

    if (!mConfig.enabled) {
        self.getLogger().info("DoubleDoor disabled by config");
        return true;
    }

    self.getLogger().warn(
        "DoubleDoor: door interaction hook is not implemented yet."
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
