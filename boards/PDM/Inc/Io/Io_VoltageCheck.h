#include "App_CanTx.h"
#include "App_InRangeCheck.h"

/**
 * Initialize the callback functions shared among all voltage checks
 * @param can_tx_interface CAN TX interface
 */
void Io_VoltageInRangeCheck_Init(struct PdmCanTxInterface *can_tx_interface);

float Io_VoltageInRangeCheck_GetVbatVoltage(void);
void  Io_VoltageInRangeCheck_VbatErrorCallback(enum InRangeCheck_Status status);

float Io_VoltageInRangeCheck_Get24vAuxVoltage(void);
void  Io_VoltageInRangeCheck_24vAuxErrorCallback(
     enum InRangeCheck_Status status);

float Io_VoltageInRangeCheck_Get24vAccVoltage(void);
void  Io_VoltageInRangeCheck_24vAccErrorCallback(
     enum InRangeCheck_Status status);
