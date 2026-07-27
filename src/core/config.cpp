#include "core/config.hpp"

namespace trainer {

Config Config::load_default() {
    return Config{
        .target_process_name = "Game.exe",
    };
}

}  // namespace trainer
