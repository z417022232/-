#include <iostream>
#include <string>

#include "core/cheat.hpp"
#include "core/config.hpp"
#include "core/memory.hpp"
#include "core/trainer.hpp"
#include "platform/process.hpp"

namespace {

bool has_arg(int argc, char* argv[], const std::string& name) {
    for (int index = 1; index < argc; ++index) {
        if (argv[index] == name) {
            return true;
        }
    }

    return false;
}

std::string value_after_arg(int argc, char* argv[], const std::string& name) {
    for (int index = 1; index + 1 < argc; ++index) {
        if (argv[index] == name) {
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
              << "  --help              Show this help.\n";
}

void print_cheats(const trainer::Trainer& app) {
    std::cout << "\nAvailable cheats:\n";
    for (const auto& cheat : app.cheats()) {
        std::cout << " - " << cheat.id << ": " << cheat.name
                  << " [" << (cheat.enabled ? "enabled" : "disabled") << "]\n"
                  << "   " << cheat.description << "\n";
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (has_arg(argc, argv, "--help")) {
        print_usage();
        return 0;
    }

    const auto config = trainer::Config::from_args(argc, argv);
    trainer::Trainer app;

    std::cout << "Fengling Trainer v0.1.0\n";
    std::cout << "Target process: " << config.target_process_name << "\n\n";

    if (has_arg(argc, argv, "--list")) {
        print_cheats(app);
        return 0;
    }

    const auto cheat_to_enable = value_after_arg(argc, argv, "--enable");
    if (!cheat_to_enable.empty() && !app.enable_cheat(cheat_to_enable)) {
        std::cout << "Unknown cheat id: " << cheat_to_enable << "\n";
        print_cheats(app);
        return 1;
    }

    const auto process = trainer::find_process_by_name(config.target_process_name);
    if (!process.has_value()) {
        std::cout << "Target process was not found. Start the game first, then run the trainer.\n";
        return 0;
    }

    auto handle = trainer::open_process(*process);
    if (!handle.has_value()) {
        std::cout << "Target process was found, but opening it failed. Try running as administrator.\n";
        return 1;
    }

    trainer::MemoryAccessor memory(std::move(*handle));
    std::cout << "Attached PID: " << memory.process_info().pid << "\n";

    print_cheats(app);

    return 0;
}
