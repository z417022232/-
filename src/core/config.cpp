#include "core/config.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string_view>

namespace trainer {

namespace {

std::string trim(std::string value) {
    const auto is_not_space = [](unsigned char ch) {
        return std::isspace(ch) == 0;
    };

    value.erase(value.begin(), std::ranges::find_if(value, is_not_space));
    value.erase(std::ranges::find_if(value.rbegin(), value.rend(), is_not_space).base(), value.end());
    return value;
}

void apply_key_value(Config& config, const std::string& key, const std::string& value) {
    if (key == "target_process") {
        config.target_process_name = value;
        return;
    }

    if (key == "log_path") {
        config.log_path = value;
        return;
    }

    constexpr std::string_view hotkey_prefix = "hotkey.";
    if (key.starts_with(hotkey_prefix)) {
        config.cheat_hotkeys[key.substr(hotkey_prefix.size())] = value;
    }
}

std::string value_after_arg(int argc, char* argv[], const std::string& name) {
    for (int index = 1; index + 1 < argc; ++index) {
        if (std::string_view(argv[index]) == name) {
            return argv[index + 1];
        }
    }

    return {};
}

}  // namespace

Config Config::load_default() {
    return Config{
        .target_process_name = "Game.exe",
        .log_path = "logs/fengling_trainer.log",
        .cheat_hotkeys = {
            {"infinite_health", "F1"},
            {"infinite_money", "F2"},
            {"no_cooldown", "F3"},
        },
    };
}

Config Config::from_file(const std::string& path) {
    Config config = load_default();
    if (path.empty()) {
        return config;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        const auto comment_position = line.find_first_of("#;");
        if (comment_position != std::string::npos) {
            line.erase(comment_position);
        }

        line = trim(line);
        if (line.empty() || line.front() == '[') {
            continue;
        }

        const auto separator = line.find('=');
        if (separator == std::string::npos) {
            continue;
        }

        const auto key = trim(line.substr(0, separator));
        const auto value = trim(line.substr(separator + 1));
        if (!key.empty() && !value.empty()) {
            apply_key_value(config, key, value);
        }
    }

    return config;
}

Config Config::from_args(int argc, char* argv[]) {
    Config config = from_file(value_after_arg(argc, argv, "--config"));

    for (int index = 1; index < argc; ++index) {
        const std::string arg = argv[index];
        if ((arg == "--target" || arg == "-t") && index + 1 < argc) {
            config.target_process_name = argv[++index];
        } else if (arg == "--log" && index + 1 < argc) {
            config.log_path = argv[++index];
        } else if (arg == "--config" && index + 1 < argc) {
            ++index;
        }
    }

    return config;
}

}  // namespace trainer
