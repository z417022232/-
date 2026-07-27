#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace trainer {

struct ProcessInfo {
    std::uint32_t pid = 0;
    std::string name;
};

class ProcessHandle {
public:
    ProcessHandle() = default;
    explicit ProcessHandle(ProcessInfo info);
    ProcessHandle(const ProcessHandle&) = delete;
    ProcessHandle& operator=(const ProcessHandle&) = delete;
    ProcessHandle(ProcessHandle&& other) noexcept;
    ProcessHandle& operator=(ProcessHandle&& other) noexcept;
    ~ProcessHandle();

    [[nodiscard]] bool is_open() const;
    [[nodiscard]] const ProcessInfo& info() const;

#ifdef _WIN32
    [[nodiscard]] void* native_handle() const;
#endif

private:
    void close();

    ProcessInfo info_{};
#ifdef _WIN32
    void* handle_ = nullptr;
#else
    bool open_ = false;
#endif
};

std::optional<ProcessInfo> find_process_by_name(const std::string& process_name);
std::optional<ProcessHandle> open_process(const ProcessInfo& process);

}  // namespace trainer
