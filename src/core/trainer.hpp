#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/cheat.hpp"

namespace trainer {

class Trainer {
public:
    Trainer();

    [[nodiscard]] const std::vector<Cheat>& cheats() const;
    [[nodiscard]] std::optional<Cheat> find_cheat(const std::string& id) const;

    bool enable_cheat(const std::string& id);
    bool disable_cheat(const std::string& id);

private:
    std::vector<Cheat> cheats_;
};

}  // namespace trainer
