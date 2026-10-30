#pragma once
#include <cstdint>

namespace app::tv::shared_datatypes
{
enum class DriveMode : uint8_t
{
    VANILLA,
    TRACK,
    ACCELERATION,
};
} // namespace app::tv::shared_datatypes
