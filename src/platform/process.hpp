#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace trainer {

struct ProcessInfo {
    std::uint32_t pid = 0;
    std::string name;
};

std::optional<ProcessInfo> find_process_by_name(const std::string& process_name);

}  // namespace trainer
