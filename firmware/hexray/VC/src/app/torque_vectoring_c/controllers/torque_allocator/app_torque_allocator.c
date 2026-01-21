// place holder as of now:
// responsible for allocating torque using load transfer (this should be done the same way as last year, should
// accomoadate)
#include "app_torque_allocator.h"

//--------------------- Constants and Datatypes ------------------------------------------------//

//----------------------- Static Function Prototypes -------------------------------------------//

//----------------------- Global Functions -----------------------------------------------------//

TorqueAllocationOutputs app_torque_allocator(void)
{
    TorqueAllocationOutputs out = { 0 };
    return out;
}

//----------------------- Static Function Definition -------------------------------------------//
static SensorStatus __attribute__((unused)) check_sensors(void)
{
    SensorStatus status = { 0 };
    return status;
}
