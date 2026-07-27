#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "platform/process.hpp"

namespace trainer {

struct MemoryResult {
    bool ok = false;
    std::string message;

    static MemoryResult success(std::string message = {});
    static MemoryResult failure(std::string message);
};

class MemoryAccessor {
public:
    explicit MemoryAccessor(ProcessHandle process);

    [[nodiscard]] bool is_attached() const;
    [[nodiscard]] const ProcessInfo& process_info() const;

    [[nodiscard]] std::optional<std::vector<std::uint8_t>> read_bytes(
        std::uintptr_t address,
        std::size_t size) const;

    [[nodiscard]] MemoryResult write_bytes(
        std::uintptr_t address,
        std::span<const std::uint8_t> bytes) const;

private:
    ProcessHandle process_;
};

}  // namespace trainer
