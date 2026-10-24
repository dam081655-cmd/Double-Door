
#include "mod/MyMod.h"

#include <pl/ModMenu.hpp>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

namespace clange_me {

namespace {

constexpr auto ModuleId = "double_door.stopwatch";

std::atomic<bool> running{false};
std::atomic<bool> enabled{false};
std::thread timerThread;

void updateStopwatch() {
    using clock = std::chrono::steady_clock;

    const auto start = clock::now();

    while (running.load()) {
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                clock::now() - start
            ).count();

        const auto minutes = elapsed / 60000;
        const auto seconds = (elapsed / 1000) % 60;
        const auto tenths = (elapsed / 100) % 10;

        char text[64];
        std::snprintf(
            text,
            sizeof(text),
            "Stopwatch %02lld:%02lld.%lld",
            static_cast<long long>(minutes),
            static_cast<long long>(seconds),
            static_cast<long long>(tenths)
        );

        pl::modmenu::DrawCommand command{};
        command.type = pl::modmenu::DrawCommandType::Text;
        command.x = 20.0f;
        command.y = 20.0f;
        command.text = text;

        pl::modmenu::submitDrawCommands(
            ModuleId,
            std::vector<pl::modmenu::DrawCommand>{command}
        );

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)
        );
    }
}

} // namespace

ClangeMeMod& ClangeMeMod::instance() {
    static ClangeMeMod mod;
    return mod;
}

ClangeMeMod::ClangeMeMod()
    : mSelf(*ll::mod::NativeMod::current()) {}

bool ClangeMeMod::load() {
    getSelf().getLogger().info("Stopwatch loading");
    return true;
}

bool ClangeMeMod::enable() {
    if (enabled.exchange(true)) {
        return true;
    }

    if (!pl::modmenu::ModuleBuilder(
            ModuleId, "Stopwatch HUD")
            .modId(getSelf().getId())
            .description("Displays an elapsed-time counter")
            .defaultEnabled(true)
            .registerModule()) {
        enabled.store(false);
        return false;
    }

    running.store(true);
    timerThread = std::thread(updateStopwatch);

    getSelf().getLogger().info("Stopwatch started");
    return true;
}

bool ClangeMeMod::disable() {
    enabled.store(false);
    running.store(false);

    if (timerThread.joinable()) {
        timerThread.join();
    }

    pl::modmenu::submitDrawCommands(
        ModuleId,
        std::vector<pl::modmenu::DrawCommand>{}
    );

    pl::modmenu::unregisterModule(ModuleId);
    return true;
}

bool ClangeMeMod::unload() {
    return disable();
}

} // namespace clange_me
