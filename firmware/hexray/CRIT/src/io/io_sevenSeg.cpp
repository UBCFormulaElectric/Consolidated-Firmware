#include "io_sevenSeg.hpp"
#include "hw_pwmOutputs.hpp"
#include "hw_spis.hpp"
#include <algorithm>
#include "hw_gpios.hpp"

namespace io::seven_seg
{
namespace
{
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
    digit           char_to_segment(const char cc)
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
            default:
                return dot;
        }
    }
} // namespace
result<void> write(const std::span<char, DIGITS> data)
{
    std::ranges::reverse(data);

    std::array<digit, DIGITS> ssdata{};
    for (size_t ii = 0; ii < DIGITS; ii++)
    {
        ssdata[ii] = char_to_segment(data[ii]);
    }
    RETURN_IF_ERR_SILENT(
        seven_seg_device.transmit(std::span{ reinterpret_cast<uint8_t *>(ssdata.data()), ssdata.size() }));
    seven_seg_rck.writePin(true);
    seven_seg_rck.writePin(false);
    return {};
}

result<void> setBrightness(const float brightness)
{
    RETURN_IF_ERR_SILENT(seven_seg_dimming.start());
    RETURN_IF_ERR_SILENT(seven_seg_dimming.setDutyCycle(100 - brightness));
    return {};
}
} // namespace io::seven_seg