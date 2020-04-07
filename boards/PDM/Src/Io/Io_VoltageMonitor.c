#include "App_SharedMacros.h"
#include "App_VoltageMonitor.h"
#include "Io_VoltageMonitor.h"

static struct PdmCanTxInterface *_can_tx_interface = NULL;

void Io_VoltageMonitor_Init(struct PdmCanTxInterface *can_tx_interface)
{
    _can_tx_interface = can_tx_interface;
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

void Io_VoltageMonitor_VbatErrorCallback(struct VoltageMonitor *voltage_monitor)
{
    UNUSED(voltage_monitor);
    App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_VBAT(
        _can_tx_interface, 1);
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

void Io_VoltageMonitor_24vAuxErrorCallback(
    struct VoltageMonitor *voltage_monitor)
{
    UNUSED(voltage_monitor);
    App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_24_V_AUX(
        _can_tx_interface, 1);
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

void Io_VoltageMonitor_24vAccErrorCallback(
    struct VoltageMonitor *voltage_monitor)
{
    UNUSED(voltage_monitor);
    App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT_24_V_ACC(
        _can_tx_interface, 1);
}
