#include "core/trainer.hpp"

#include <algorithm>

namespace trainer {

Trainer::Trainer()
    : cheats_{
          Cheat{"infinite_health", "Infinite health", "Keep player health from decreasing.", false},
          Cheat{"infinite_money", "Infinite money", "Keep money/resources from decreasing.", false},
          Cheat{"no_cooldown", "No skill cooldown", "Skip skill cooldown timers.", false},
      } {}

const std::vector<Cheat>& Trainer::cheats() const {
    return cheats_;
}

std::optional<Cheat> Trainer::find_cheat(const std::string& id) const {
    const auto it = std::ranges::find_if(cheats_, [&](const Cheat& cheat) {
        return cheat.id == id;
    });

    if (it == cheats_.end()) {
        return std::nullopt;
    }

    return *it;
}

bool Trainer::enable_cheat(const std::string& id) {
    const auto it = std::ranges::find_if(cheats_, [&](const Cheat& cheat) {
        return cheat.id == id;
    });

    if (it == cheats_.end()) {
        return false;
    }

    it->enable();
    return true;
}

bool Trainer::disable_cheat(const std::string& id) {
    const auto it = std::ranges::find_if(cheats_, [&](const Cheat& cheat) {
        return cheat.id == id;
    });

    if (it == cheats_.end()) {
        return false;
    }

    it->disable();
    return true;
}

}  // namespace trainer
