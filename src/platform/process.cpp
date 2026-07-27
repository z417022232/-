#include "platform/process.hpp"

#include <utility>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace trainer {

ProcessHandle::ProcessHandle(ProcessInfo info)
    : info_(std::move(info)) {
#ifdef _WIN32
    handle_ = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_LIMITED_INFORMATION,
                          FALSE,
                          info_.pid);
#else
    open_ = info_.pid != 0;
#endif
}

ProcessHandle::ProcessHandle(ProcessHandle&& other) noexcept
    : info_(std::move(other.info_)) {
#ifdef _WIN32
    handle_ = other.handle_;
    other.handle_ = nullptr;
#else
    open_ = other.open_;
    other.open_ = false;
#endif
}

ProcessHandle& ProcessHandle::operator=(ProcessHandle&& other) noexcept {
    if (this != &other) {
        close();
        info_ = std::move(other.info_);
#ifdef _WIN32
        handle_ = other.handle_;
        other.handle_ = nullptr;
#else
        open_ = other.open_;
        other.open_ = false;
#endif
    }

    return *this;
}

ProcessHandle::~ProcessHandle() {
    close();
}

bool ProcessHandle::is_open() const {
#ifdef _WIN32
    return handle_ != nullptr;
#else
    return open_;
#endif
}

const ProcessInfo& ProcessHandle::info() const {
    return info_;
}

#ifdef _WIN32
void* ProcessHandle::native_handle() const {
    return handle_;
}
#endif

void ProcessHandle::close() {
#ifdef _WIN32
    if (handle_ != nullptr) {
        CloseHandle(handle_);
        handle_ = nullptr;
    }
#else
    open_ = false;
#endif
}

std::optional<ProcessInfo> find_process_by_name(const std::string& process_name) {
#ifdef _WIN32
    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return std::nullopt;
    }

    PROCESSENTRY32A entry{};
    entry.dwSize = sizeof(PROCESSENTRY32A);

    if (!Process32FirstA(snapshot, &entry)) {
        CloseHandle(snapshot);
        return std::nullopt;
    }

    do {
        if (process_name == entry.szExeFile) {
            ProcessInfo info{
                .pid = entry.th32ProcessID,
                .name = entry.szExeFile,
            };
            CloseHandle(snapshot);
            return info;
        }
    } while (Process32NextA(snapshot, &entry));

    CloseHandle(snapshot);
#else
    (void)process_name;
#endif

    return std::nullopt;
}

std::optional<ProcessHandle> open_process(const ProcessInfo& process) {
    ProcessHandle handle(process);
    if (!handle.is_open()) {
        return std::nullopt;
    }

    return handle;
}

}  // namespace trainer
