#include <iostream>
#include <string_view>
#include <string>

#include "core/cheat.hpp"
#include "core/config.hpp"
#include "core/logger.hpp"
#include "core/memory.hpp"
#include "core/trainer.hpp"
#include "core/version.hpp"
#include "platform/process.hpp"

namespace {

bool has_arg(int argc, char* argv[], const std::string& name) {
    for (int index = 1; index < argc; ++index) {
        if (std::string_view(argv[index]) == name) {
            return true;
        }
    }

    return false;
}

std::string value_after_arg(int argc, char* argv[], const std::string& name) {
    for (int index = 1; index + 1 < argc; ++index) {
        if (std::string_view(argv[index]) == name) {
            return argv[index + 1];
        }
    }

    return {};
}

void print_usage() {
    std::cout << "Usage:\n"
              << "  fengling_trainer --target Game.exe --list\n"
              << "  fengling_trainer --target Game.exe --enable infinite_health\n\n"
              << "Options:\n"
              << "  -t, --target <exe>   Target process name. Default: Game.exe\n"
              << "  --list              List available cheats.\n"
              << "  --enable <id>       Enable a cheat by id.\n"
              << "  --disable <id>      Disable a cheat by id.\n"
              << "  --toggle <id>       Toggle a cheat by id.\n"
              << "  --version           Show version.\n"
              << "  --help              Show this help.\n";
}

void print_cheats(const trainer::Trainer& app) {
    std::cout << "\nAvailable cheats:\n";
    for (const auto& cheat : app.cheats()) {
        std::cout << " - " << cheat.id << ": " << cheat.name
                  << " (" << cheat.hotkey << ")"
                  << " [" << (cheat.enabled ? "enabled" : "disabled") << "]\n"
                  << "   " << cheat.description << "\n";
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    trainer::Logger logger("logs/fengling_trainer.log");

    if (has_arg(argc, argv, "--help")) {
        print_usage();
        return 0;
    }

    if (has_arg(argc, argv, "--version")) {
        std::cout << trainer::kAppName << " v" << trainer::kAppVersion << "\n";
        return 0;
    }

    const auto config = trainer::Config::from_args(argc, argv);
    trainer::Trainer app;

    logger.info("Application started.");
    logger.info("Target process: " + config.target_process_name);

    std::cout << trainer::kAppName << " v" << trainer::kAppVersion << "\n";
    std::cout << "Target process: " << config.target_process_name << "\n\n";

    const auto cheat_to_enable = value_after_arg(argc, argv, "--enable");
    if (!cheat_to_enable.empty() && !app.enable_cheat(cheat_to_enable)) {
        std::cout << "Unknown cheat id: " << cheat_to_enable << "\n";
        logger.warning("Unknown cheat id for enable: " + cheat_to_enable);
        print_cheats(app);
        return 1;
    }

    const auto cheat_to_disable = value_after_arg(argc, argv, "--disable");
    if (!cheat_to_disable.empty() && !app.disable_cheat(cheat_to_disable)) {
        std::cout << "Unknown cheat id: " << cheat_to_disable << "\n";
        logger.warning("Unknown cheat id for disable: " + cheat_to_disable);
        print_cheats(app);
        return 1;
    }

    const auto cheat_to_toggle = value_after_arg(argc, argv, "--toggle");
    if (!cheat_to_toggle.empty() && !app.toggle_cheat(cheat_to_toggle)) {
        std::cout << "Unknown cheat id: " << cheat_to_toggle << "\n";
        logger.warning("Unknown cheat id for toggle: " + cheat_to_toggle);
        print_cheats(app);
        return 1;
    }

    if (has_arg(argc, argv, "--list")) {
        print_cheats(app);
        return 0;
    }

    const auto process = trainer::find_process_by_name(config.target_process_name);
    if (!process.has_value()) {
        std::cout << "Target process was not found. Start the game first, then run the trainer.\n";
        logger.warning("Target process was not found.");
        return 0;
    }

    auto handle = trainer::open_process(*process);
    if (!handle.has_value()) {
        std::cout << "Target process was found, but opening it failed. Try running as administrator.\n";
        logger.error("Target process was found, but opening it failed.");
        return 1;
    }

    trainer::MemoryAccessor memory(std::move(*handle));
    std::cout << "Attached PID: " << memory.process_info().pid << "\n";
    logger.info("Attached PID: " + std::to_string(memory.process_info().pid));

    print_cheats(app);

    return 0;
}
