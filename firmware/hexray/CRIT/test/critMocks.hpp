#pragma once
#include <array>
#include "io_sevenSeg.hpp"

namespace fakes
{
namespace seven_seg
{
    std::array<char, io::seven_seg::DIGITS> getData();
} // namespace seven_seg
} // namespace fakes