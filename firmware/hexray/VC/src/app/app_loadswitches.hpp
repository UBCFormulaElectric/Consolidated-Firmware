#pragma once
#include <cstddef>
// constexpr inline size_t NUM_EFUSE_CHANNELS = 10;

namespace app::loadswitches
{
struct EfuseCanMsg
{
    io::Efuse &efuse;
    void (*can_setter_enabled)(bool);
    void (*can_setter_current)(float);
};
extern EfuseCanMsg efuse_channel_setters;
    
void efuse_broadcast();
} // namespace app::loadswitches
