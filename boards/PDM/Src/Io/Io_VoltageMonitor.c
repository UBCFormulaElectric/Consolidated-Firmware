#include <string.h>

#include "App_VoltageMonitor.h"
#include "Io_VoltageMonitor.h"

static struct PDMCanTxInterface *_can_tx_interface = NULL;

void Io_VoltageMonitor_Init(struct PDMCanTxInterface *can_tx_interface)
{
    _can_tx_interface = can_tx_interface;
}

void Io_VoltageMonitor_ErrorCallback(struct VoltageMonitor *voltage_monitor)
{
    const char *name = App_VoltageMonitor_GetName(voltage_monitor);

    if (strcmp(name, "INIT") == 0)
    {
        App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_VBAT(
            _can_tx_interface, 1);
    }
    else if (strcmp(name, "AUX") == 0)
    {
        App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_24_V_AUX(
            _can_tx_interface, 1);
    }
    else if (strcmp(name, "ACC") == 0)
    {
        App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_24_V_ACC(
            _can_tx_interface, 1);
    }
}

float Io_VoltageMonitor_GetVbatVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 7.0f;
}

float Io_VoltageMonitor_GetVbatMinVoltage(void)
{
    return 6.0f;
}

float Io_VoltageMonitor_GetVbatMaxVoltage(void)
{
    return 8.5f;
}

float Io_VoltageMonitor_Get24vAccVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

float Io_VoltageMonitor_Get24vAccMinVoltage(void)
{
    return 22.0f;
}

float Io_VoltageMonitor_Get24vAccMaxVoltage(void)
{
    return 26.0f;
}

float Io_VoltageMonitor_Get24vAuxVoltage(void)
{
    // TODO: Fix hard-coded value once the ADC is configured
    return 23.0f;
}

float Io_VoltageMonitor_Get24vAuxMinVoltage(void)
{
    return 22.0f;
}

float Io_VoltageMonitor_Get24vAuxMaxVoltage(void)
{
    return 26.0f;
}
