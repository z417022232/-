#pragma once

#include <map>
#include <string>

namespace trainer {

struct Config {
    std::string target_process_name;
    std::string log_path;
    std::map<std::string, std::string> cheat_hotkeys;

    static Config load_default();
    static Config from_file(const std::string& path);
    static Config from_args(int argc, char* argv[]);
};

}  // namespace trainer
