#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace trainer {

enum class ValueType {
    Int32,
    Float32,
    Byte,
};

struct CheatPatch {
    std::string cheat_id;
    std::uintptr_t address = 0;
    ValueType value_type = ValueType::Int32;
    std::string value;
};

struct GameProfile {
    std::string name;
    std::string process_name;
    std::vector<CheatPatch> patches;

    [[nodiscard]] std::optional<CheatPatch> patch_for(const std::string& cheat_id) const;
};

struct ProfileLoadResult {
    GameProfile profile;
    std::vector<std::string> warnings;
    bool loaded = false;
};

[[nodiscard]] ProfileLoadResult load_profile(const std::string& path);
[[nodiscard]] std::string value_type_name(ValueType type);
[[nodiscard]] std::vector<std::uint8_t> encode_patch_value(const CheatPatch& patch);

}  // namespace trainer
