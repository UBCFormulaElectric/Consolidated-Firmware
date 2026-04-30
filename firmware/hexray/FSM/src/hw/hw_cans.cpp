#include "hw_cans.hpp"
#include <cassert>
#include "main.h"
#ifndef USE_CHIMERA
#include "jobs.hpp"
#endif

namespace hw::can
{
const fdcan fdcan1{ hfdcan1, [](const CanMsg &msg)
                    {
#ifndef USE_CHIMERA
                        jobs_runCanRxCallback(msg);
#endif
                    } }; // define callback func in io
} // namespace hw::can

namespace hw
{
const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can::fdcan1.getHfdcan());
    return can::fdcan1;
}
} // namespace hw