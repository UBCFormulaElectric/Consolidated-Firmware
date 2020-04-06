#include "Io_VoltageMonitor.h"

static struct PDMCanTxInterface *_can_tx_interface = NULL;

void Io_VoltageMonitor_Init(struct PDMCanTxInterface *can_tx_interface)
{
    _can_tx_interface = can_tx_interface;
}

void Io_VoltageMonitor_voltage_out_of_range_callback(
    struct VoltageMonitor *voltage_monitor)
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
}
