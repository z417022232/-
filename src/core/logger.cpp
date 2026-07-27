#include "core/logger.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace trainer {

namespace {

std::string current_timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    std::ostringstream output;
    output << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

}  // namespace

Logger::Logger(std::filesystem::path path)
    : path_(std::move(path)) {
    if (path_.has_parent_path()) {
        std::filesystem::create_directories(path_.parent_path());
    }

    stream_.open(path_, std::ios::app);
}

void Logger::info(const std::string& message) {
    write("INFO", message);
}

void Logger::warning(const std::string& message) {
    write("WARN", message);
}

void Logger::error(const std::string& message) {
    write("ERROR", message);
}

const std::filesystem::path& Logger::path() const {
    return path_;
}

void Logger::write(const std::string& level, const std::string& message) {
    if (!stream_.is_open()) {
        return;
    }

    stream_ << "[" << current_timestamp() << "] [" << level << "] " << message << "\n";
}

}  // namespace trainer
