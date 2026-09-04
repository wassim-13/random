#include "utils.h"

#include <charconv>

int64_t get_number(std::string_view str, int default_val) {
    int result = 0;

    auto [ptr, ec] =
        std::from_chars(str.data(), str.data() + str.size(), result);

    if (ec == std::errc{} && ptr == str.data() + str.size()) {
        return result;
    }

    return default_val;
}
