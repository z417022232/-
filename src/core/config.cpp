#include "core/config.hpp"

namespace trainer {

Config Config::load_default() {
    return Config{
        .target_process_name = "Game.exe",
    };
}

Config Config::from_args(int argc, char* argv[]) {
    Config config = load_default();

    for (int index = 1; index < argc; ++index) {
        const std::string arg = argv[index];
        if ((arg == "--target" || arg == "-t") && index + 1 < argc) {
            config.target_process_name = argv[++index];
        }
    }

    return config;
}

}  // namespace trainer
