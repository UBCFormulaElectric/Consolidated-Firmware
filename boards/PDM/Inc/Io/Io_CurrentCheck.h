#include "App_CanTx.h"
#include "App_InRangeCheck.h"

/**
 * Register the CAN TX interface to be used in each error callback function
 * @param can_tx_interface CAN TX interface
 */
void Io_CurrentInRangeCheck_Init(struct PdmCanTxInterface *can_tx);

float Io_CurrentInRangeCheck_GetAux1Current(void);
void  Io_CurrentInRangeCheck_Aux1ErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetAux2Current(void);
void  Io_CurrentInRangeCheck_Aux2ErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetLeftInverterCurrent(void);
void  Io_CurrentInRangeCheck_LeftInverterErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetRightInverterCurrent(void);
void  Io_CurrentInRangeCheck_RightInverterErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetEnergyMeterCurrent(void);
void  Io_CurrentInRangeCheck_EnergyMeterErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetCanCurrent(void);
void  Io_CurrentInRangeCheck_CanErrorCallback(
     enum InRangeCheck_ErrorStatus status);

float Io_CurrentInRangeCheck_GetAirShutdownCurrent(void);
void  Io_CurrentInRangeCheck_AirShutdownErrorCallback(
     enum InRangeCheck_ErrorStatus status);
