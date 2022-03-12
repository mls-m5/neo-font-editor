#pragma once

#include <array>
#include <locale>
#include <optional>

namespace impl {

inline auto locale = std::locale{"sv_SE.UTF8"};

}

inline bool isUtfTail(char c) {
    // 0xC0 = 0b11000000
    // 0x80 = 0b10000000
    return ((c & 0xC0) == 0x80);
}

inline char toLower(char c) {
    c = std::tolower(c, impl::locale);
    constexpr auto chars = std::array<std::pair<char, char>, 4>{{
        {"å"[0], "Å"[0]},
        {"ä"[0], "Ä"[0]},
        {"ö"[0], "Ö"[0]},
        {"é"[0], "É"[0]},
    }};

    for (auto &p : chars) {
        if (p.second == c) {
            c = p.first;
        }
    }

    return c;
}

inline bool isSwedishSpecialChar(char c) {
    constexpr auto swedishChars = std::array{
        "å"[0],
        "ä"[0],
        "ö"[0],
        "é"[0],
        "Å"[0],
        "Ä"[0],
        "É"[0],
    };
    for (auto sc : swedishChars) {
        if (sc == c) {
            return true;
        }
    }
    return false;
}

inline std::string toLower(std::string str) {
    for (auto &c : str) {
        c = toLower(c);
    }
    return str;
}

inline bool isAlphaNoSpace(char c) {
    return (std::isalpha(c, impl::locale) || isUtfTail(c) ||
            isSwedishSpecialChar(c)) &&
           !std::isspace(c);
}

inline std::string trim(std::string str) {
    while (!str.empty() && std::isspace(str.front())) {
        str.erase(0, 1);
    }

    while (!str.empty() && std::isspace(str.back())) {
        str.pop_back();
    }

    return str;
}

inline std::optional<std::pair<std::string, std::string>> split(
    std::string str, std::string separator) {
    auto f = str.find(separator);

    if (f == std::string::npos) {
        return std::nullopt;
    }

    return std::pair{str.substr(0, f), str.substr(f + separator.size())};
}
