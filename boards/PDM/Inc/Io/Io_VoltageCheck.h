#include "App_CanTx.h"

struct InRangeCheck;

/**
 * Initialize the callback functions shared among all voltage checks
 * @param can_tx_interface CAN TX interface
 */
void Io_VoltageInRangeCheck_Init(struct PdmCanTxInterface *can_tx_interface);

float Io_VoltageInRangeCheck_GetVbatVoltage(void);
void  Io_VoltageInRangeCheck_VbatErrorCallback(
     struct InRangeCheck *in_range_check);

float Io_VoltageInRangeCheck_Get24vAuxVoltage(void);
void  Io_VoltageInRangeCheck_24vAuxErrorCallback(
     struct InRangeCheck *in_range_check);

float Io_VoltageInRangeCheck_Get24vAccVoltage(void);
void  Io_VoltageInRangeCheck_24vAccErrorCallback(
     struct InRangeCheck *in_range_check);
