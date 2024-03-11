#include "VC.h"
#include "utils.h"

extern "C"
{
#include "app_stateMachine.h"
#include "states/app_initState.h"
}

namespace VC
{

__VISIBLE void init(void)
{
    app_stateMachine_init(app_initState_get());
}
__VISIBLE void tick1Hz(void)
{
    app_stateMachine_tick1Hz();
}
__VISIBLE void tick100Hz(void)
{
    app_stateMachine_tick100Hz();
}

} // namespace VC