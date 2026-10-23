#pragma once

#include <optional>
#include <memory>

#include "mod/Config.h"

#include <pl/Mod.hpp>
#include <pl/memory/Hook.hpp>

namespace clange_me {

class ClangeMeMod {
public:
    static ClangeMeMod &instance();

    ClangeMeMod();

    [[nodiscard]] ll::mod::NativeMod &getSelf() const {
        return mSelf;
    }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod &mSelf;
    ModConfig mConfig;
    std::optional<pl::config::ConfigFile<ModConfig>> mConfigFile;

    std::unique_ptr<pl::memory::HookHandle> mDoorHook;
};

} // namespace clange_me
