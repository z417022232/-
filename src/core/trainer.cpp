#include "core/trainer.hpp"

#include <algorithm>
#include <map>

namespace trainer {

Trainer::Trainer()
    : cheats_{
          Cheat{"infinite_health", "Infinite health", "Keep player health from decreasing.", "F1", false},
          Cheat{"infinite_money", "Infinite money", "Keep money/resources from decreasing.", "F2", false},
          Cheat{"no_cooldown", "No skill cooldown", "Skip skill cooldown timers.", "F3", false},
      } {}

Trainer::Trainer(const std::map<std::string, std::string>& hotkeys)
    : Trainer() {
    apply_hotkeys(hotkeys);
}

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

bool Trainer::toggle_cheat(const std::string& id) {
    const auto it = std::ranges::find_if(cheats_, [&](const Cheat& cheat) {
        return cheat.id == id;
    });

    if (it == cheats_.end()) {
        return false;
    }

    if (it->enabled) {
        it->disable();
    } else {
        it->enable();
    }

    return true;
}

void Trainer::apply_hotkeys(const std::map<std::string, std::string>& hotkeys) {
    for (auto& cheat : cheats_) {
        const auto hotkey = hotkeys.find(cheat.id);
        if (hotkey != hotkeys.end()) {
            cheat.hotkey = hotkey->second;
        }
    }
}

}  // namespace trainer
