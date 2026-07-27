#include "core/memory.hpp"

#include <utility>

#ifdef _WIN32
#include <windows.h>
#endif

namespace trainer {

MemoryResult MemoryResult::success(std::string message) {
    return MemoryResult{
        .ok = true,
        .message = std::move(message),
    };
}

MemoryResult MemoryResult::failure(std::string message) {
    return MemoryResult{
        .ok = false,
        .message = std::move(message),
    };
}

MemoryAccessor::MemoryAccessor(ProcessHandle process)
    : process_(std::move(process)) {}

bool MemoryAccessor::is_attached() const {
    return process_.is_open();
}

const ProcessInfo& MemoryAccessor::process_info() const {
    return process_.info();
}

std::optional<std::vector<std::uint8_t>> MemoryAccessor::read_bytes(
    std::uintptr_t address,
    std::size_t size) const {
    if (!process_.is_open() || address == 0 || size == 0) {
        return std::nullopt;
    }

    std::vector<std::uint8_t> buffer(size);

#ifdef _WIN32
    SIZE_T bytes_read = 0;
    const BOOL ok = ReadProcessMemory(
        process_.native_handle(),
        reinterpret_cast<LPCVOID>(address),
        buffer.data(),
        buffer.size(),
        &bytes_read);

    if (!ok || bytes_read != buffer.size()) {
        return std::nullopt;
    }

    return buffer;
#else
    (void)address;
    return std::nullopt;
#endif
}

MemoryResult MemoryAccessor::write_bytes(
    std::uintptr_t address,
    std::span<const std::uint8_t> bytes) const {
    if (!process_.is_open()) {
        return MemoryResult::failure("No target process is attached.");
    }

    if (address == 0 || bytes.empty()) {
        return MemoryResult::failure("Invalid memory write request.");
    }

#ifdef _WIN32
    SIZE_T bytes_written = 0;
    const BOOL ok = WriteProcessMemory(
        process_.native_handle(),
        reinterpret_cast<LPVOID>(address),
        bytes.data(),
        bytes.size(),
        &bytes_written);

    if (!ok || bytes_written != bytes.size()) {
        return MemoryResult::failure("WriteProcessMemory failed.");
    }

    return MemoryResult::success("Memory write completed.");
#else
    (void)address;
    (void)bytes;
    return MemoryResult::failure("Memory writes are only implemented on Windows.");
#endif
}

}  // namespace trainer
