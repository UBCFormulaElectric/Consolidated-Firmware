#pragma once
#include <cstddef>
// constexpr inline size_t NUM_EFUSE_CHANNELS = 10;
namespace app::loadswitches
{
// extern EfuseCanMsg efuse_channel_setters;

void efuse_broadcast();
} // namespace app::loadswitches
