#include "App_VoltageMonitor.h"
#include "auto_generated/App_CanTx.h"

void Io_VoltageMonitor_Init(struct PDMCanTxInterface *can_tx_interface);
void Io_VoltageMonitor_voltage_out_of_range_callback(
    struct VoltageMonitor *voltage_monitor);
