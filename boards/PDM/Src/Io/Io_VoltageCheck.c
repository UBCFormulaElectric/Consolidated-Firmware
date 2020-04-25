#include "App_InRangeCheck.h"
#include "Io_VoltageCheck.h"

static struct PdmCanTxInterface *_can_tx = NULL;

void Io_VoltageInRangeCheck_Init(struct PdmCanTxInterface *can_tx)
{
    _can_tx = can_tx;
}

float Io_VoltageInRangeCheck_GetVbatVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 7.0f;
}

void Io_VoltageInRangeCheck_VbatErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_VBAT(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_VBAT(_can_tx, true);
    }
}

float Io_VoltageInRangeCheck_Get24vAuxVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

void Io_VoltageInRangeCheck_24vAuxErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_AUX(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_AUX(_can_tx, true);
    }
}

float Io_VoltageInRangeCheck_Get24vAccVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

void Io_VoltageInRangeCheck_24vAccErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_ACC(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_ACC(_can_tx, true);
    }
}
