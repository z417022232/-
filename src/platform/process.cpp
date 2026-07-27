#include "platform/process.hpp"

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace trainer {

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

}  // namespace trainer
