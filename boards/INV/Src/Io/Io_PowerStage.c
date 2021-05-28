
#include "Io_PowerStage.h"
#include "main.h"

bool Io_PowerStage_GetPhaOCFault(void)
{
    if(HAL_GPIO_ReadPin(nPHA_OC_ALARM_GPIO_Port, nPHA_OC_ALARM_Pin))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Io_PowerStage_GetPhbOCFault(void)
{
    if(HAL_GPIO_ReadPin(nPHB_OC_ALARM_GPIO_Port, nPHB_OC_ALARM_Pin))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Io_PowerStage_GetPhcOCFault(void)
{
    if(HAL_GPIO_ReadPin(nPHC_OC_ALARM_GPIO_Port, nPHC_OC_ALARM_Pin))
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Io_PowerStage_GetPowerStageOTFault(void)
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


