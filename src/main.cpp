#include <iostream>
#include <vector>

#include "core/cheat.hpp"
#include "core/config.hpp"
#include "platform/process.hpp"

int main() {
    const auto config = trainer::Config::load_default();

    std::cout << "Fengling Trainer v0.1.0\n";
    std::cout << "Target process: " << config.target_process_name << "\n\n";

    const auto process = trainer::find_process_by_name(config.target_process_name);
    if (!process.has_value()) {
        std::cout << "Target process was not found. Start the game first, then run the trainer.\n";
        return 0;
    }

    std::cout << "Attached candidate PID: " << process->pid << "\n";

    std::vector<trainer::Cheat> cheats{
        trainer::Cheat{"infinite_health", "Infinite health", false},
        trainer::Cheat{"infinite_money", "Infinite money", false},
        trainer::Cheat{"no_cooldown", "No skill cooldown", false},
    };

    std::cout << "\nAvailable cheats:\n";
    for (const auto& cheat : cheats) {
        std::cout << " - " << cheat.id << ": " << cheat.name
                  << " [" << (cheat.enabled ? "enabled" : "disabled") << "]\n";
    }

    return 0;
}
