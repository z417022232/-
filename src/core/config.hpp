#pragma once

#include <string>

namespace trainer {

struct Config {
    std::string target_process_name;

    static Config load_default();
};

}  // namespace trainer
