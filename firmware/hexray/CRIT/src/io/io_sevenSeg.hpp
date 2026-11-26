#pragma once
#include "util_errorCodes.hpp"
#include <cstdio>
#include <span>

namespace io::seven_seg
{
inline constexpr size_t DIGITS = 9;

__attribute__((packed)) struct digit
{
    bool d : 1   = false;
    bool e : 1   = false;
    bool g : 1   = false;
    bool f : 1   = false;
    bool a : 1   = false;
    bool b : 1   = false;
    bool dot : 1 = false;
    bool c : 1   = false;
    constexpr explicit digit(
        const bool _a,
        const bool _b,
        const bool _c,
        const bool _d,
        const bool _e,
        const bool _f,
        const bool _g,
        const bool _dot)
      : d(_d), e(_e), g(_g), f(_f), a(_a), b(_b), dot(_dot), c(_c)
    {
    }

    constexpr digit() = default;
};
static_assert(sizeof(digit) == 1);

constexpr digit zero{ true, true, true, true, true, true, false, false };
constexpr digit one{ false, true, true, false, false, false, false, false };
constexpr digit two{ true, true, false, true, true, false, true, false };
constexpr digit three{ true, true, true, true, false, false, true, false };
constexpr digit four{ false, true, true, false, false, true, true, false };
constexpr digit five{ true, false, true, true, false, true, true, false };
constexpr digit six{ true, false, true, true, true, true, true, false };
constexpr digit seven{ true, true, true, false, false, false, false, false };
constexpr digit eight{ true, true, true, true, true, true, true, false };
constexpr digit nine{ true, true, true, false, false, true, true, false };
constexpr digit a{ true, true, true, false, true, true, true, false };
constexpr digit b{ false, false, true, true, true, true, true, false };
constexpr digit c{ true, false, false, true, true, true, false, false };
constexpr digit d{ false, true, true, true, true, false, true, false };
constexpr digit e{ true, false, false, true, true, true, true, false };
constexpr digit f{ true, false, false, false, true, true, true, false };
constexpr digit g{ true, true, true, true, false, true, true, false };
constexpr digit h{ false, false, true, false, true, true, true, false };
constexpr digit i{ false, false, false, false, true, false, false, false };
constexpr digit j{ false, false, true, true, false, false, false, false };
constexpr digit k{ false, false, false, false, true, true, true, false };
constexpr digit l{ false, false, false, true, true, true, false, false };
constexpr digit m{ true, false, true, false, true, false, false, false };
constexpr digit n{ false, false, true, false, true, false, true, false };
constexpr digit o{ false, false, true, true, true, false, true, false };
constexpr digit p{ true, true, false, false, true, true, true, false };
constexpr digit q{ true, true, true, false, false, true, true, false };
constexpr digit r{ false, false, false, false, true, false, true, false };
constexpr digit s{ true, false, true, true, false, true, true, false };
constexpr digit t{ false, false, false, true, true, true, true, false };
constexpr digit u{ false, true, true, true, true, true, false, false };
constexpr digit v{ false, true, false, true, false, true, false, false };
constexpr digit y{ false, true, true, true, false, true, true, false };
constexpr digit dot{ false, false, false, false, false, false, false, true };
constexpr digit space{ false, false, false, false, false, false, false, false };

inline digit char_to_segment(const char cc)
{
    switch (cc)
    {
        case '0':
            return zero;
        case '1':
            return one;
        case '2':
            return two;
        case '3':
            return three;
        case '4':
            return four;
        case '5':
            return five;
        case '6':
            return six;
        case '7':
            return seven;
        case '8':
            return eight;
        case '9':
            return nine;
        case 'a':
        case 'A':
            return a;
        case 'b':
        case 'B':
            return b;
        case 'c':
        case 'C':
            return c;
        case 'd':
        case 'D':
            return d;
        case 'e':
        case 'E':
            return e;
        case 'f':
        case 'F':
            return f;
        case 'g':
        case 'G':
            return g;
        case 'h':
        case 'H':
            return h;
        case 'i':
        case 'I':
            return i;
        case 'j':
        case 'J':
            return j;
        case 'k':
        case 'K':
            return k;
        case 'l':
        case 'L':
            return l;
        case 'm':
        case 'M':
            return m;
        case 'n':
        case 'N':
            return n;
        case 'o':
        case 'O':
            return o;
        case 'p':
        case 'P':
            return p;
        case 'q':
        case 'Q':
            return q;
        case 'r':
        case 'R':
            return r;
        case 's':
        case 'S':
            return s;
        case 't':
        case 'T':
            return t;
        case 'u':
        case 'U':
            return u;
        case 'v':
        case 'V':
            return v;
        case 'y':
        case 'Y':
            return y;
        case '.':
            return dot;
        case ' ':
        default:
            return space;
    }
}

/**
 * Write the given value to the seven segment display
 * @note that the data will be touched, so please consider the buffer invalidated after use
 * @return success of operation
 */
[[nodiscard]] result<void> write(std::span<char, DIGITS> data);
/**
 * Set the brightness of the seven segment display.
 * @param brightness brightness value from 0.0 (off) to 1.0 (max brightness)
 * @return success of operation
 */
[[nodiscard]] result<void> setBrightness(float brightness);
} // namespace io::seven_seg
