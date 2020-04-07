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
    enum VoltageMonitor_Status status =
        App_VoltageMonitor_GetStatus(voltage_monitor);

    switch (status)
    {
        case VOLTAGEMONITOR_UNDERVOLTAGE:
        {
            App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT(
                _can_tx_interface, 1);
        }
        break;
        case VOLTAGEMONITOR_OVERVOLTAGE:
        {
            App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT(
                _can_tx_interface, 1);
        }
        break;
        default:
        {
        }
        break;
    }

    App_CanTx_SetPeriodicSignal_VOLTAGE_MONITOR_FAULT(_can_tx_interface, 1);

    const char *name = App_VoltageMonitor_GetName(voltage_monitor);

    struct CanMsgs_pdm_errors_payload_t payload;
    payload.error_id =
        CANMSGS_PDM_ERRORS_PAYLOAD_ERROR_ID_VOLTAGE_MONITOR_FAULT_CHOICE;

    if (strcmp(name, "INIT") == 0)
    {
        payload.voltage_monitor_name =
            CANMSGS_PDM_ERRORS_PAYLOAD_VOLTAGE_MONITOR_NAME_VBAT_CHOICE;
    }
    else if (strcmp(name, "ACC") == 0)
    {
        payload.voltage_monitor_name =
            CANMSGS_PDM_ERRORS_PAYLOAD_VOLTAGE_MONITOR_NAME_24_V_AUX_CHOICE;
    }
    else if (strcmp(name, "AUX") == 0)
    {
        payload.voltage_monitor_name =
            CANMSGS_PDM_ERRORS_PAYLOAD_VOLTAGE_MONITOR_NAME_24_V_ACC_CHOICE;
    }

    App_CanTx_SendNonPeriodicMsg_PDM_ERRORS_PAYLOAD(_can_tx_interface, &payload);
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
