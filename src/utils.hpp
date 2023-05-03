#pragma once

#include <stdexcept>
#include <string>

namespace pascal2c {

enum class Color {
    Reset,
    Red,
    Green,
    Yellow,
    Blue,
};

inline const char* ColorCode(Color color) {
    switch (color) {
        case Color::Reset:
            return "\033[0m";
        case Color::Red:
            return "\033[31m";
        case Color::Green:
            return "\033[32m";
        case Color::Yellow:
            return "\033[33m";
        case Color::Blue:
            return "\033[34m";
        default:
            throw std::runtime_error("Unsupport color");
    }
}

inline std::string Colorize(const std::string& str, Color color) {
    return std::string(ColorCode(color)) + str + ColorCode(Color::Reset);
}

}  // namespace pascal2c