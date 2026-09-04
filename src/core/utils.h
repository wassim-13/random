#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <vector>

int64_t get_number(std::string_view str, int default_val);
std::vector<std::string> split_whitespace(std::string_view str);

constexpr uint32_t hash_str(std::string_view str) {
    uint32_t hash = 2166136261u;
    for (char c : str) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

constexpr uint32_t operator""_h(const char* str, size_t len) {
    return hash_str(std::string_view(str, len));
}

#endif
