#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/cheat.hpp"

namespace trainer {

class Trainer {
public:
    Trainer();
    explicit Trainer(const std::map<std::string, std::string>& hotkeys);

    [[nodiscard]] const std::vector<Cheat>& cheats() const;
    [[nodiscard]] std::optional<Cheat> find_cheat(const std::string& id) const;

    bool enable_cheat(const std::string& id);
    bool disable_cheat(const std::string& id);
    bool toggle_cheat(const std::string& id);

private:
    std::vector<Cheat> cheats_;

    void apply_hotkeys(const std::map<std::string, std::string>& hotkeys);
};

}  // namespace trainer
