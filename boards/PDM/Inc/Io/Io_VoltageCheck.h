#include "App_CanTx.h"
#include "App_InRangeCheck.h"

/**
 * Register the CAN TX interface to be used in each error callback function
 * @param can_tx_interface CAN TX interface
 */
void Io_VoltageInRangeCheck_Init(struct PdmCanTxInterface *can_tx);

float Io_VoltageInRangeCheck_GetVbatVoltage(void);
void  Io_VoltageInRangeCheck_VbatErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_VoltageInRangeCheck_Get24vAuxVoltage(void);
void  Io_VoltageInRangeCheck_24vAuxErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_VoltageInRangeCheck_Get24vAccVoltage(void);
void  Io_VoltageInRangeCheck_24vAccErrorCallback(
     enum InRangeCheck_ErrorStatus status);
