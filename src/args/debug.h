#ifndef DEBUG_H
#define DEBUG_H

#include <print>

#if !defined(NDEBUG) || defined(DEBUG)
constexpr bool is_debug_build = true;
#else
constexpr bool is_debug_build = false;
#endif

struct DebugConfig {
    static inline bool verbose = false;

    [[nodiscard]] static constexpr bool is_enabled() noexcept {
        if constexpr (is_debug_build) {
            return true;
        }
        return verbose;
    }
};

inline void set_verbose(bool enable) {
    DebugConfig::verbose = enable;
}

namespace detail {
template <typename... Args>
void dbg_impl(std::format_string<Args...> fmt, Args&&... args) {
    std::print("[DEBUG] ");
    std::println(fmt, std::forward<Args>(args)...);
}

template <typename T>
    requires(!std::is_convertible_v<T, std::string_view>)
void dbg_impl(const T& val) {
    std::println("[DEBUG] {}", val);
}
} // namespace detail

#define dbg(...)                                                               \
    do {                                                                       \
        if (DebugConfig::is_enabled()) {                                       \
            ::detail::dbg_impl(__VA_ARGS__);                                   \
        }                                                                      \
    } while (0)

#endif
