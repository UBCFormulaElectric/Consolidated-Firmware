#include "App_CanTx.h"

struct InRangeCheck;

/**
 * Initialize the callback functions shared among all current checks
 * @param can_tx_interface CAN TX interface
 */
void Io_CurrentInRangeCheck_Init(struct PdmCanTxInterface *can_tx_interface);

float Io_CurrentInRangeCheck_GetAux1Current(void);
void  Io_CurrentInRangeCheck_Aux1ErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetAux2Current(void);
void  Io_CurrentInRangeCheck_Aux2ErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetLeftInverterCurrent(void);
void  Io_CurrentInRangeCheck_LeftInverterErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetRightInverterCurrent(void);
void  Io_CurrentInRangeCheck_RightInverterErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetEnergyMeterCurrent(void);
void  Io_CurrentInRangeCheck_EnergyMeterErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetCanCurrent(void);
void  Io_CurrentInRangeCheck_CanErrorCallback(
     struct InRangeCheck *current_check);

float Io_CurrentInRangeCheck_GetAirShutdownCurrent(void);
void  Io_CurrentInRangeCheck_AirShutdownErrorCallback(
     struct InRangeCheck *current_check);
