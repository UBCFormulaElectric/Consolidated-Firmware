#include "Io_CurrentCheck.h"

static struct PdmCanTxInterface *_can_tx = NULL;

// Convenient function to set the appropriate CAN signals in each error callback
static void App_SetOutOfRangeCanSignals(
    enum InRangeCheck_ErrorStatus status,
    void (*const underflow_setter)(struct PdmCanTxInterface *, uint8_t),
    void (*const overflow_setter)(struct PdmCanTxInterface *, uint8_t))
{
    switch (status)
    {
        case VALUE_UNDERFLOW:
        {
            underflow_setter(_can_tx, true);
        }
        break;
        case VALUE_OVERFLOW:
        {
            overflow_setter(_can_tx, true);
        }
        break;
    }
}

void Io_CurrentInRangeCheck_Init(struct PdmCanTxInterface *can_tx)
{
    _can_tx = can_tx;
}

float Io_CurrentInRangeCheck_GetAux1Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux1ErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_1,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_1);
}

float Io_CurrentInRangeCheck_GetAux2Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux2ErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_2,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_2);
}

float Io_CurrentInRangeCheck_GetLeftInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_LeftInverterErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_LEFT_INVERTER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_LEFT_INVERTER);
}

float Io_CurrentInRangeCheck_GetRightInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_RightInverterErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_RIGHT_INVERTER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_RIGHT_INVERTER);
}

float Io_CurrentInRangeCheck_GetEnergyMeterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_EnergyMeterErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_ENERGY_METER,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_ENERGY_METER);
}

float Io_CurrentInRangeCheck_GetCanCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_CanErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_CAN,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_CAN);
}

float Io_CurrentInRangeCheck_GetAirShutdownCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_AirShutdownErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERCURRENT_AIR_SHUTDOWN,
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AIR_SHUTDOWN);
}
