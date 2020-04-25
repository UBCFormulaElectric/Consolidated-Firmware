#include "App_InRangeCheck.h"
#include "Io_CurrentCheck.h"

static struct PdmCanTxInterface *_can_tx = NULL;

void Io_CurrentInRangeCheck_Init(struct PdmCanTxInterface *can_tx)
{
    _can_tx = can_tx;
}

float Io_CurrentInRangeCheck_GetAux1Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux1ErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_1(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_1(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetAux2Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux2ErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_2(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_2(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetLeftInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_LeftInverterErrorCallback(
    enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_LEFT_INVERTER(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_LEFT_INVERTER(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetRightInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_RightInverterErrorCallback(
    enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_RIGHT_INVERTER(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_RIGHT_INVERTER(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetEnergyMeterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_EnergyMeterErrorCallback(
    enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_ENERGY_METER(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_ENERGY_METER(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetCanCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_CanErrorCallback(enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_CAN(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_CAN(_can_tx, true);
    }
}

float Io_CurrentInRangeCheck_GetAirShutdownCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_AirShutdownErrorCallback(
    enum InRangeCheck_Status status)
{
    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AIR_SHUTDOWN(_can_tx, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AIR_SHUTDOWN(_can_tx, true);
    }
}
