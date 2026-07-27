#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace trainer {

class Logger {
public:
    explicit Logger(std::filesystem::path path);

    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

    [[nodiscard]] const std::filesystem::path& path() const;

private:
    void write(const std::string& level, const std::string& message);

    std::filesystem::path path_;
    std::ofstream stream_;
};

}  // namespace trainer
