
#include "Io_ECI1118.h"
#include "main.h"



bool Io_ECI1118_GetMotorOTFault(void)
{
    if(HAL_GPIO_ReadPin(nMOD_OT_ALARM_GPIO_Port, nMOD_OT_ALARM_Pin))
    {
        return false;
    }
    else
    {
        return true;
    }
}

uint32_t Io_ECI1118_GetMotorPosition(void)
{

}
