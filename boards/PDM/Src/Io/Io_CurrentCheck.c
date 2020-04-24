#include "App_InRangeCheck.h"
#include "Io_CurrentCheck.h"

static struct PdmCanTxInterface *_can_tx_interface = NULL;

void Io_CurrentInRangeCheck_Init(struct PdmCanTxInterface *can_tx_interface)
{
    _can_tx_interface = can_tx_interface;
}

float Io_CurrentInRangeCheck_GetAux1Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux1ErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_1(_can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_1(_can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetAux2Current(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_Aux2ErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AUX_2(_can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AUX_2(_can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetLeftInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_LeftInverterErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_LEFT_INVERTER(
            _can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_LEFT_INVERTER(
            _can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetRightInverterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_RightInverterErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_RIGHT_INVERTER(
            _can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_RIGHT_INVERTER(
            _can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetEnergyMeterCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_EnergyMeterErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_ENERGY_METER(
            _can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_ENERGY_METER(
            _can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetCanCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_CanErrorCallback(struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_CAN(_can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_CAN(_can_tx_interface, true);
    }
}

float Io_CurrentInRangeCheck_GetAirShutdownCurrent(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 0.5f;
}

void Io_CurrentInRangeCheck_AirShutdownErrorCallback(
    struct InRangeCheck *current_check)
{
    enum InRangeCheck_Status status = App_InRangeCheck_GetStatus(current_check);

    if (status == VALUE_UNDERFLOW)
    {
        App_CanTx_SetPeriodicSignal_UNDERCURRENT_AIR_SHUTDOWN(
            _can_tx_interface, true);
    }
    else if (status == VALUE_OVERFLOW)
    {
        App_CanTx_SetPeriodicSignal_OVERCURRENT_AIR_SHUTDOWN(
            _can_tx_interface, true);
    }
}
