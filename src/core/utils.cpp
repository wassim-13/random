#include "utils.h"

#include <charconv>

using namespace std;

int64_t get_number(std::string_view str, int default_val) {
    int result = 0;

    auto [ptr, ec] =
        std::from_chars(str.data(), str.data() + str.size(), result);

    if (ec == std::errc{} && ptr == str.data() + str.size()) {
        return result;
    }

    return default_val;
}

vector<string> split_whitespace(string_view str) {
    std::vector<std::string> words;
    size_t i = 0;
    const size_t len = str.size();

    while (i < len) {

        while (i < len && std::isspace(static_cast<unsigned char>(str[i]))) {
            ++i;
        }
        if (i >= len)
            break;

        size_t start = i;
        while (i < len && !std::isspace(static_cast<unsigned char>(str[i]))) {
            ++i;
        }

        words.emplace_back(str.substr(start, i - start));
    }

    return words;
}
