#include "Io_VoltageCheck.h"
#include "App_InRangeCheck.h"

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

void Io_VoltageInRangeCheck_Init(struct PdmCanTxInterface *can_tx)
{
    _can_tx = can_tx;
}

float Io_VoltageInRangeCheck_GetVbatVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 7.0f;
}

void Io_VoltageInRangeCheck_VbatErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_VBAT,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_VBAT);
}

float Io_VoltageInRangeCheck_Get24vAuxVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

void Io_VoltageInRangeCheck_24vAuxErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_AUX,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_AUX);
}

float Io_VoltageInRangeCheck_Get24vAccVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

void Io_VoltageInRangeCheck_24vAccErrorCallback(
    enum InRangeCheck_ErrorStatus status)
{
    App_SetOutOfRangeCanSignals(
        status, App_CanTx_SetPeriodicSignal_UNDERVOLTAGE_24_V_ACC,
        App_CanTx_SetPeriodicSignal_OVERVOLTAGE_24_V_ACC);
}
