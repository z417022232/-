#include "core/profile.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstring>
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

std::optional<std::uintptr_t> parse_address(const std::string& value) {
    const int base = value.starts_with("0x") || value.starts_with("0X") ? 16 : 10;
    std::uintptr_t result = 0;
    const char* begin = value.data() + (base == 16 ? 2 : 0);
    const char* end = value.data() + value.size();
    const auto parsed = std::from_chars(begin, end, result, base);
    if (parsed.ec != std::errc{} || parsed.ptr != end) {
        return std::nullopt;
    }

    return result;
}

std::optional<ValueType> parse_value_type(const std::string& value) {
    if (value == "int32") {
        return ValueType::Int32;
    }

    if (value == "float32") {
        return ValueType::Float32;
    }

    if (value == "byte") {
        return ValueType::Byte;
    }

    return std::nullopt;
}

void apply_patch_key(
    std::map<std::string, CheatPatch>& patches,
    std::vector<std::string>& warnings,
    const std::string& key,
    const std::string& value) {
    constexpr std::string_view prefix = "cheat.";
    if (!key.starts_with(prefix)) {
        return;
    }

    const auto field_separator = key.find('.', prefix.size());
    if (field_separator == std::string::npos) {
        warnings.push_back("Ignored malformed profile key: " + key);
        return;
    }

    const auto cheat_id = key.substr(prefix.size(), field_separator - prefix.size());
    const auto field = key.substr(field_separator + 1);
    auto& patch = patches[cheat_id];
    patch.cheat_id = cheat_id;

    if (field == "address") {
        const auto address = parse_address(value);
        if (!address.has_value()) {
            warnings.push_back("Invalid address for " + cheat_id + ": " + value);
            return;
        }
        patch.address = *address;
        return;
    }

    if (field == "type") {
        const auto type = parse_value_type(value);
        if (!type.has_value()) {
            warnings.push_back("Invalid value type for " + cheat_id + ": " + value);
            return;
        }
        patch.value_type = *type;
        return;
    }

    if (field == "value") {
        patch.value = value;
        return;
    }

    warnings.push_back("Ignored unknown profile field: " + key);
}

template <typename T>
std::vector<std::uint8_t> bytes_of(T value) {
    std::vector<std::uint8_t> bytes(sizeof(T));
    std::memcpy(bytes.data(), &value, sizeof(T));
    return bytes;
}

}  // namespace

std::optional<CheatPatch> GameProfile::patch_for(const std::string& cheat_id) const {
    const auto it = std::ranges::find_if(patches, [&](const CheatPatch& patch) {
        return patch.cheat_id == cheat_id;
    });

    if (it == patches.end()) {
        return std::nullopt;
    }

    return *it;
}

ProfileLoadResult load_profile(const std::string& path) {
    ProfileLoadResult result;
    if (path.empty()) {
        return result;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        result.warnings.push_back("Profile file could not be opened: " + path);
        return result;
    }

    result.loaded = true;
    std::map<std::string, CheatPatch> patches;
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
            result.warnings.push_back("Ignored malformed profile line: " + line);
            continue;
        }

        const auto key = trim(line.substr(0, separator));
        const auto value = trim(line.substr(separator + 1));
        if (key == "profile_name") {
            result.profile.name = value;
        } else if (key == "process_name") {
            result.profile.process_name = value;
        } else {
            apply_patch_key(patches, result.warnings, key, value);
        }
    }

    for (const auto& [cheat_id, patch] : patches) {
        if (patch.address == 0) {
            result.warnings.push_back("Missing address for " + cheat_id);
        }
        if (patch.value.empty()) {
            result.warnings.push_back("Missing value for " + cheat_id);
        }
        result.profile.patches.push_back(patch);
    }

    return result;
}

std::string value_type_name(ValueType type) {
    switch (type) {
        case ValueType::Int32:
            return "int32";
        case ValueType::Float32:
            return "float32";
        case ValueType::Byte:
            return "byte";
    }

    return "unknown";
}

std::vector<std::uint8_t> encode_patch_value(const CheatPatch& patch) {
    switch (patch.value_type) {
        case ValueType::Int32: {
            const auto value = static_cast<std::int32_t>(std::stol(patch.value));
            return bytes_of(value);
        }
        case ValueType::Float32: {
            const auto value = std::stof(patch.value);
            return bytes_of(value);
        }
        case ValueType::Byte: {
            const auto value = static_cast<std::uint8_t>(std::stoul(patch.value));
            return {value};
        }
    }

    return {};
}

}  // namespace trainer
