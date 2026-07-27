#pragma once

#include <string>

namespace trainer {

struct Cheat {
    std::string id;
    std::string name;
    bool enabled = false;

    void enable();
    void disable();
};

}  // namespace trainer
