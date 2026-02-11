#pragma once

#include <cstdint>

namespace fakes::time
{
void setTimeFake(uint32_t ms);
}

#ifdef __cplusplus
extern "C"
{
#endif

    void     fake_io_time_getCurrentMs_returns(uint32_t ms);
    uint32_t io_time_getCurrentMs(void);

#ifdef __cplusplus
}
#endif