/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#pragma once

/* ------------------------------- Includes ------------------------------- */

#include "app_canUtils.h"

/* ------------------------- Function Prototypes -------------------------- */

/**
 * Initialize TX signals to their starting values.
 */
void app_canTx_init(void);



/**
 * Update value stored in TX table of signal VC_Heartbeat in msg VC_Vitals.
 */
void app_canTx_VC_Heartbeat_set(bool value);
/**
 * Update value stored in TX table of signal VC_State in msg VC_Vitals.
 */
void app_canTx_VC_State_set(VcState value);
/**
 * Update value stored in TX table of signal VC_BuzzerOn in msg VC_Vitals.
 */
void app_canTx_VC_BuzzerOn_set(bool value);
/**
 * Update value stored in TX table of signal VC_NumberOfCanDataLogs in msg VC_Vitals.
 */
void app_canTx_VC_NumberOfCanDataLogs_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_CanLoggingRemainingErrors in msg VC_Vitals.
 */
void app_canTx_VC_CanLoggingRemainingErrors_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_LeftInverterTorqueCommand in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterTorqueCommand_set(float value);
/**
 * Update value stored in TX table of signal VC_LeftInverterSpeedCommand in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterSpeedCommand_set(int value);
/**
 * Update value stored in TX table of signal VC_LeftInverterDirectionCommand in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterDirectionCommand_set(InverterDirection value);
/**
 * Update value stored in TX table of signal VC_LeftInverterEnable in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterEnable_set(bool value);
/**
 * Update value stored in TX table of signal VC_LeftInverterEnableDischarge in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterEnableDischarge_set(bool value);
/**
 * Update value stored in TX table of signal VC_LeftInverterEnableSpeedMode in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterEnableSpeedMode_set(bool value);
/**
 * Update value stored in TX table of signal VC_LeftInverterTorqueLimit in msg VC_LeftInverterCommand.
 */
void app_canTx_VC_LeftInverterTorqueLimit_set(float value);
/**
 * Update value stored in TX table of signal VC_INVL_CommandParameterAddress in msg VC_INVL_ReadWriteParamCommand.
 */
void app_canTx_VC_INVL_CommandParameterAddress_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_INVL_CommandReadWrite in msg VC_INVL_ReadWriteParamCommand.
 */
void app_canTx_VC_INVL_CommandReadWrite_set(InverterReadWriteCommand value);
/**
 * Update value stored in TX table of signal VC_INVL_CommandData in msg VC_INVL_ReadWriteParamCommand.
 */
void app_canTx_VC_INVL_CommandData_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_RightInverterTorqueCommand in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterTorqueCommand_set(float value);
/**
 * Update value stored in TX table of signal VC_RightInverterSpeedCommand in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterSpeedCommand_set(int value);
/**
 * Update value stored in TX table of signal VC_RightInverterDirectionCommand in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterDirectionCommand_set(InverterDirection value);
/**
 * Update value stored in TX table of signal VC_RightInverterEnable in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterEnable_set(bool value);
/**
 * Update value stored in TX table of signal VC_RightInverterEnableDischarge in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterEnableDischarge_set(bool value);
/**
 * Update value stored in TX table of signal VC_RightInverterEnableSpeedMode in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterEnableSpeedMode_set(bool value);
/**
 * Update value stored in TX table of signal VC_RightInverterTorqueLimit in msg VC_RightInverterCommand.
 */
void app_canTx_VC_RightInverterTorqueLimit_set(float value);
/**
 * Update value stored in TX table of signal VC_INVR_CommandParameterAddress in msg VC_INVR_ReadWriteParamCommand.
 */
void app_canTx_VC_INVR_CommandParameterAddress_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_INVR_CommandReadWrite in msg VC_INVR_ReadWriteParamCommand.
 */
void app_canTx_VC_INVR_CommandReadWrite_set(InverterReadWriteCommand value);
/**
 * Update value stored in TX table of signal VC_INVR_CommandData in msg VC_INVR_ReadWriteParamCommand.
 */
void app_canTx_VC_INVR_CommandData_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_WatchdogTimeout in msg VC_AlertsContext.
 */
void app_canTx_VC_WatchdogTimeout_set(bool value);
/**
 * Update value stored in TX table of signal VC_WatchdogTimeoutTaskName in msg VC_AlertsContext.
 */
void app_canTx_VC_WatchdogTimeoutTaskName_set(RtosTaskName value);
/**
 * Update value stored in TX table of signal VC_TxOverflowCount in msg VC_AlertsContext.
 */
void app_canTx_VC_TxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_RxOverflowCount in msg VC_AlertsContext.
 */
void app_canTx_VC_RxOverflowCount_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_EllipseGeneralStatusBitmask in msg VC_EllipseStatus.
 */
void app_canTx_VC_EllipseGeneralStatusBitmask_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_EllipseComStatusBitmask in msg VC_EllipseStatus.
 */
void app_canTx_VC_EllipseComStatusBitmask_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_EkfSolutionMode in msg VC_EllipseStatus.
 */
void app_canTx_VC_EkfSolutionMode_set(VcEkfStatus value);
/**
 * Update value stored in TX table of signal VC_EllipseTimestamp in msg VC_EllipseTime.
 */
void app_canTx_VC_EllipseTimestamp_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_EulerAnglesRoll in msg VC_EllipseEulerAngles.
 */
void app_canTx_VC_EulerAnglesRoll_set(float value);
/**
 * Update value stored in TX table of signal VC_EulerAnglesPitch in msg VC_EllipseEulerAngles.
 */
void app_canTx_VC_EulerAnglesPitch_set(float value);
/**
 * Update value stored in TX table of signal VC_EulerAnglesYaw in msg VC_EllipseEulerAngles.
 */
void app_canTx_VC_EulerAnglesYaw_set(float value);
/**
 * Update value stored in TX table of signal VC_Hash in msg VC_CommitInfo.
 */
void app_canTx_VC_Hash_set(uint32_t value);
/**
 * Update value stored in TX table of signal VC_Clean in msg VC_CommitInfo.
 */
void app_canTx_VC_Clean_set(bool value);
/**
 * Update value stored in TX table of signal VC_PowerEstimateValue in msg VC_PowerEstimate.
 */
void app_canTx_VC_PowerEstimateValue_set(float value);
/**
 * Update value stored in TX table of signal VC_PowerEstimateMeasuredValue in msg VC_PowerEstimate.
 */
void app_canTx_VC_PowerEstimateMeasuredValue_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDPowerEstimateOutput in msg VC_PowerEstimatePID.
 */
void app_canTx_VC_PIDPowerEstimateOutput_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDPowerEstimateError in msg VC_PowerEstimatePID.
 */
void app_canTx_VC_PIDPowerEstimateError_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDPowerEstimateDerivative in msg VC_PowerEstimatePID.
 */
void app_canTx_VC_PIDPowerEstimateDerivative_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDPowerEstimateIntegral in msg VC_PowerEstimatePID.
 */
void app_canTx_VC_PIDPowerEstimateIntegral_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDSlipRatioOutput in msg VC_PIDSlipRatio.
 */
void app_canTx_VC_PIDSlipRatioOutput_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDSlipRatioError in msg VC_PIDSlipRatio.
 */
void app_canTx_VC_PIDSlipRatioError_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDSlipRatioDerivative in msg VC_PIDSlipRatio.
 */
void app_canTx_VC_PIDSlipRatioDerivative_set(float value);
/**
 * Update value stored in TX table of signal VC_PIDSlipRatioIntegral in msg VC_PIDSlipRatio.
 */
void app_canTx_VC_PIDSlipRatioIntegral_set(float value);
/**
 * Update value stored in TX table of signal VC_SlipRatioLeft in msg VC_SlipRatios.
 */
void app_canTx_VC_SlipRatioLeft_set(float value);
/**
 * Update value stored in TX table of signal VC_SlipRatioRight in msg VC_SlipRatios.
 */
void app_canTx_VC_SlipRatioRight_set(float value);
/**
 * Update value stored in TX table of signal VC_ActiveDiffTorqueLeft in msg VC_ActiveDiffOutputs.
 */
void app_canTx_VC_ActiveDiffTorqueLeft_set(float value);
/**
 * Update value stored in TX table of signal VC_ActiveDiffTorqueRight in msg VC_ActiveDiffOutputs.
 */
void app_canTx_VC_ActiveDiffTorqueRight_set(float value);
/**
 * Update value stored in TX table of signal VC_PowerLimitValue in msg VC_PowerLimit.
 */
void app_canTx_VC_PowerLimitValue_set(float value);
/**
 * Update value stored in TX table of signal VC_PowerLimitValueAtFullThrottle in msg VC_PowerLimit.
 */
void app_canTx_VC_PowerLimitValueAtFullThrottle_set(float value);
/**
 * Update value stored in TX table of signal VC_RegenEnabled in msg VC_VehicleDynamics.
 */
void app_canTx_VC_RegenEnabled_set(bool value);
/**
 * Update value stored in TX table of signal VC_TorqueVectoringEnabled in msg VC_VehicleDynamics.
 */
void app_canTx_VC_TorqueVectoringEnabled_set(bool value);
/**
 * Update value stored in TX table of signal VC_VehicleVelocity in msg VC_VehicleDynamics.
 */
void app_canTx_VC_VehicleVelocity_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityNorth in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityNorth_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityNorthAccuracy in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityNorthAccuracy_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityEast in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityEast_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityEastAccuracy in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityEastAccuracy_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityDown in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityDown_set(float value);
/**
 * Update value stored in TX table of signal VC_VelocityDownAccuracy in msg VC_EllipseEkfNavVelocity.
 */
void app_canTx_VC_VelocityDownAccuracy_set(float value);
/**
 * Update value stored in TX table of signal VC_BatVoltage in msg VC_Voltages.
 */
void app_canTx_VC_BatVoltage_set(float value);
/**
 * Update value stored in TX table of signal VC_BoostVoltage in msg VC_Voltages.
 */
void app_canTx_VC_BoostVoltage_set(float value);
/**
 * Update value stored in TX table of signal VC_AccVoltage in msg VC_Voltages.
 */
void app_canTx_VC_AccVoltage_set(float value);
/**
 * Update value stored in TX table of signal VC_AccCurrent in msg VC_CurrentSensing.
 */
void app_canTx_VC_AccCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_BatCurrent in msg VC_CurrentSensing.
 */
void app_canTx_VC_BatCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_ShdnCurrent in msg VC_Efuse.
 */
void app_canTx_VC_ShdnCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_ShdnStatus in msg VC_Efuse.
 */
void app_canTx_VC_ShdnStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_LvCurrent in msg VC_Efuse.
 */
void app_canTx_VC_LvCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_LvStatus in msg VC_Efuse.
 */
void app_canTx_VC_LvStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_PumpCurrent in msg VC_Efuse.
 */
void app_canTx_VC_PumpCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_PumpStatus in msg VC_Efuse.
 */
void app_canTx_VC_PumpStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_AuxCurrent in msg VC_Efuse.
 */
void app_canTx_VC_AuxCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_AuxStatus in msg VC_Efuse.
 */
void app_canTx_VC_AuxStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_InvRCurrent in msg VC_Efuse.
 */
void app_canTx_VC_InvRCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_InvRStatus in msg VC_Efuse.
 */
void app_canTx_VC_InvRStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_InvLCurrent in msg VC_Efuse.
 */
void app_canTx_VC_InvLCurrent_set(float value);
/**
 * Update value stored in TX table of signal VC_InvLStatus in msg VC_Efuse.
 */
void app_canTx_VC_InvLStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_ImuAccelerationX in msg VC_ImuLinearAcceleration.
 */
void app_canTx_VC_ImuAccelerationX_set(float value);
/**
 * Update value stored in TX table of signal VC_ImuAccelerationY in msg VC_ImuLinearAcceleration.
 */
void app_canTx_VC_ImuAccelerationY_set(float value);
/**
 * Update value stored in TX table of signal VC_ImuAccelerationZ in msg VC_ImuLinearAcceleration.
 */
void app_canTx_VC_ImuAccelerationZ_set(float value);
/**
 * Update value stored in TX table of signal VC_FlowRate in msg VC_CoolantPumpFlowRate.
 */
void app_canTx_VC_FlowRate_set(float value);
/**
 * Update value stored in TX table of signal VC_LEStopOKStatus in msg VC_VCShdnNodeStatus.
 */
void app_canTx_VC_LEStopOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_REStopOKStatus in msg VC_VCShdnNodeStatus.
 */
void app_canTx_VC_REStopOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_TSMSOKStatus in msg VC_VCShdnNodeStatus.
 */
void app_canTx_VC_TSMSOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_SplitterBoxInterlockOKStatus in msg VC_VCShdnNodeStatus.
 */
void app_canTx_VC_SplitterBoxInterlockOKStatus_set(bool value);
/**
 * Update value stored in TX table of signal VC_FirstFaultNode in msg VC_GlobalShdnNodeStatus.
 */
void app_canTx_VC_FirstFaultNode_set(ShutdownNode value);
/**
 * Update value stored in TX table of signal VC_MappedPedalPercentage in msg VC_PedalPercentage.
 */
void app_canTx_VC_MappedPedalPercentage_set(float value);


/**
 * Get value stored in TX table of signal VC_Heartbeat in msg VC_Vitals.
 */
bool app_canTx_VC_Vitals_VC_Heartbeat_get(void);
/**
 * Get value stored in TX table of signal VC_State in msg VC_Vitals.
 */
VcState app_canTx_VC_Vitals_VC_State_get(void);
/**
 * Get value stored in TX table of signal VC_BuzzerOn in msg VC_Vitals.
 */
bool app_canTx_VC_Vitals_VC_BuzzerOn_get(void);
/**
 * Get value stored in TX table of signal VC_NumberOfCanDataLogs in msg VC_Vitals.
 */
uint32_t app_canTx_VC_Vitals_VC_NumberOfCanDataLogs_get(void);
/**
 * Get value stored in TX table of signal VC_CanLoggingRemainingErrors in msg VC_Vitals.
 */
uint32_t app_canTx_VC_Vitals_VC_CanLoggingRemainingErrors_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterTorqueCommand in msg VC_LeftInverterCommand.
 */
float app_canTx_VC_LeftInverterCommand_VC_LeftInverterTorqueCommand_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterSpeedCommand in msg VC_LeftInverterCommand.
 */
int app_canTx_VC_LeftInverterCommand_VC_LeftInverterSpeedCommand_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterDirectionCommand in msg VC_LeftInverterCommand.
 */
InverterDirection app_canTx_VC_LeftInverterCommand_VC_LeftInverterDirectionCommand_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterEnable in msg VC_LeftInverterCommand.
 */
bool app_canTx_VC_LeftInverterCommand_VC_LeftInverterEnable_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterEnableDischarge in msg VC_LeftInverterCommand.
 */
bool app_canTx_VC_LeftInverterCommand_VC_LeftInverterEnableDischarge_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterEnableSpeedMode in msg VC_LeftInverterCommand.
 */
bool app_canTx_VC_LeftInverterCommand_VC_LeftInverterEnableSpeedMode_get(void);
/**
 * Get value stored in TX table of signal VC_LeftInverterTorqueLimit in msg VC_LeftInverterCommand.
 */
float app_canTx_VC_LeftInverterCommand_VC_LeftInverterTorqueLimit_get(void);
/**
 * Get value stored in TX table of signal VC_INVL_CommandParameterAddress in msg VC_INVL_ReadWriteParamCommand.
 */
uint32_t app_canTx_VC_INVL_ReadWriteParamCommand_VC_INVL_CommandParameterAddress_get(void);
/**
 * Get value stored in TX table of signal VC_INVL_CommandReadWrite in msg VC_INVL_ReadWriteParamCommand.
 */
InverterReadWriteCommand app_canTx_VC_INVL_ReadWriteParamCommand_VC_INVL_CommandReadWrite_get(void);
/**
 * Get value stored in TX table of signal VC_INVL_CommandData in msg VC_INVL_ReadWriteParamCommand.
 */
uint32_t app_canTx_VC_INVL_ReadWriteParamCommand_VC_INVL_CommandData_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterTorqueCommand in msg VC_RightInverterCommand.
 */
float app_canTx_VC_RightInverterCommand_VC_RightInverterTorqueCommand_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterSpeedCommand in msg VC_RightInverterCommand.
 */
int app_canTx_VC_RightInverterCommand_VC_RightInverterSpeedCommand_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterDirectionCommand in msg VC_RightInverterCommand.
 */
InverterDirection app_canTx_VC_RightInverterCommand_VC_RightInverterDirectionCommand_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterEnable in msg VC_RightInverterCommand.
 */
bool app_canTx_VC_RightInverterCommand_VC_RightInverterEnable_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterEnableDischarge in msg VC_RightInverterCommand.
 */
bool app_canTx_VC_RightInverterCommand_VC_RightInverterEnableDischarge_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterEnableSpeedMode in msg VC_RightInverterCommand.
 */
bool app_canTx_VC_RightInverterCommand_VC_RightInverterEnableSpeedMode_get(void);
/**
 * Get value stored in TX table of signal VC_RightInverterTorqueLimit in msg VC_RightInverterCommand.
 */
float app_canTx_VC_RightInverterCommand_VC_RightInverterTorqueLimit_get(void);
/**
 * Get value stored in TX table of signal VC_INVR_CommandParameterAddress in msg VC_INVR_ReadWriteParamCommand.
 */
uint32_t app_canTx_VC_INVR_ReadWriteParamCommand_VC_INVR_CommandParameterAddress_get(void);
/**
 * Get value stored in TX table of signal VC_INVR_CommandReadWrite in msg VC_INVR_ReadWriteParamCommand.
 */
InverterReadWriteCommand app_canTx_VC_INVR_ReadWriteParamCommand_VC_INVR_CommandReadWrite_get(void);
/**
 * Get value stored in TX table of signal VC_INVR_CommandData in msg VC_INVR_ReadWriteParamCommand.
 */
uint32_t app_canTx_VC_INVR_ReadWriteParamCommand_VC_INVR_CommandData_get(void);
/**
 * Get value stored in TX table of signal VC_WatchdogTimeout in msg VC_AlertsContext.
 */
bool app_canTx_VC_AlertsContext_VC_WatchdogTimeout_get(void);
/**
 * Get value stored in TX table of signal VC_WatchdogTimeoutTaskName in msg VC_AlertsContext.
 */
RtosTaskName app_canTx_VC_AlertsContext_VC_WatchdogTimeoutTaskName_get(void);
/**
 * Get value stored in TX table of signal VC_TxOverflowCount in msg VC_AlertsContext.
 */
uint32_t app_canTx_VC_AlertsContext_VC_TxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal VC_RxOverflowCount in msg VC_AlertsContext.
 */
uint32_t app_canTx_VC_AlertsContext_VC_RxOverflowCount_get(void);
/**
 * Get value stored in TX table of signal VC_EllipseGeneralStatusBitmask in msg VC_EllipseStatus.
 */
uint32_t app_canTx_VC_EllipseStatus_VC_EllipseGeneralStatusBitmask_get(void);
/**
 * Get value stored in TX table of signal VC_EllipseComStatusBitmask in msg VC_EllipseStatus.
 */
uint32_t app_canTx_VC_EllipseStatus_VC_EllipseComStatusBitmask_get(void);
/**
 * Get value stored in TX table of signal VC_EkfSolutionMode in msg VC_EllipseStatus.
 */
VcEkfStatus app_canTx_VC_EllipseStatus_VC_EkfSolutionMode_get(void);
/**
 * Get value stored in TX table of signal VC_EllipseTimestamp in msg VC_EllipseTime.
 */
uint32_t app_canTx_VC_EllipseTime_VC_EllipseTimestamp_get(void);
/**
 * Get value stored in TX table of signal VC_EulerAnglesRoll in msg VC_EllipseEulerAngles.
 */
float app_canTx_VC_EllipseEulerAngles_VC_EulerAnglesRoll_get(void);
/**
 * Get value stored in TX table of signal VC_EulerAnglesPitch in msg VC_EllipseEulerAngles.
 */
float app_canTx_VC_EllipseEulerAngles_VC_EulerAnglesPitch_get(void);
/**
 * Get value stored in TX table of signal VC_EulerAnglesYaw in msg VC_EllipseEulerAngles.
 */
float app_canTx_VC_EllipseEulerAngles_VC_EulerAnglesYaw_get(void);
/**
 * Get value stored in TX table of signal VC_Hash in msg VC_CommitInfo.
 */
uint32_t app_canTx_VC_CommitInfo_VC_Hash_get(void);
/**
 * Get value stored in TX table of signal VC_Clean in msg VC_CommitInfo.
 */
bool app_canTx_VC_CommitInfo_VC_Clean_get(void);
/**
 * Get value stored in TX table of signal VC_PowerEstimateValue in msg VC_PowerEstimate.
 */
float app_canTx_VC_PowerEstimate_VC_PowerEstimateValue_get(void);
/**
 * Get value stored in TX table of signal VC_PowerEstimateMeasuredValue in msg VC_PowerEstimate.
 */
float app_canTx_VC_PowerEstimate_VC_PowerEstimateMeasuredValue_get(void);
/**
 * Get value stored in TX table of signal VC_PIDPowerEstimateOutput in msg VC_PowerEstimatePID.
 */
float app_canTx_VC_PowerEstimatePID_VC_PIDPowerEstimateOutput_get(void);
/**
 * Get value stored in TX table of signal VC_PIDPowerEstimateError in msg VC_PowerEstimatePID.
 */
float app_canTx_VC_PowerEstimatePID_VC_PIDPowerEstimateError_get(void);
/**
 * Get value stored in TX table of signal VC_PIDPowerEstimateDerivative in msg VC_PowerEstimatePID.
 */
float app_canTx_VC_PowerEstimatePID_VC_PIDPowerEstimateDerivative_get(void);
/**
 * Get value stored in TX table of signal VC_PIDPowerEstimateIntegral in msg VC_PowerEstimatePID.
 */
float app_canTx_VC_PowerEstimatePID_VC_PIDPowerEstimateIntegral_get(void);
/**
 * Get value stored in TX table of signal VC_PIDSlipRatioOutput in msg VC_PIDSlipRatio.
 */
float app_canTx_VC_PIDSlipRatio_VC_PIDSlipRatioOutput_get(void);
/**
 * Get value stored in TX table of signal VC_PIDSlipRatioError in msg VC_PIDSlipRatio.
 */
float app_canTx_VC_PIDSlipRatio_VC_PIDSlipRatioError_get(void);
/**
 * Get value stored in TX table of signal VC_PIDSlipRatioDerivative in msg VC_PIDSlipRatio.
 */
float app_canTx_VC_PIDSlipRatio_VC_PIDSlipRatioDerivative_get(void);
/**
 * Get value stored in TX table of signal VC_PIDSlipRatioIntegral in msg VC_PIDSlipRatio.
 */
float app_canTx_VC_PIDSlipRatio_VC_PIDSlipRatioIntegral_get(void);
/**
 * Get value stored in TX table of signal VC_SlipRatioLeft in msg VC_SlipRatios.
 */
float app_canTx_VC_SlipRatios_VC_SlipRatioLeft_get(void);
/**
 * Get value stored in TX table of signal VC_SlipRatioRight in msg VC_SlipRatios.
 */
float app_canTx_VC_SlipRatios_VC_SlipRatioRight_get(void);
/**
 * Get value stored in TX table of signal VC_ActiveDiffTorqueLeft in msg VC_ActiveDiffOutputs.
 */
float app_canTx_VC_ActiveDiffOutputs_VC_ActiveDiffTorqueLeft_get(void);
/**
 * Get value stored in TX table of signal VC_ActiveDiffTorqueRight in msg VC_ActiveDiffOutputs.
 */
float app_canTx_VC_ActiveDiffOutputs_VC_ActiveDiffTorqueRight_get(void);
/**
 * Get value stored in TX table of signal VC_PowerLimitValue in msg VC_PowerLimit.
 */
float app_canTx_VC_PowerLimit_VC_PowerLimitValue_get(void);
/**
 * Get value stored in TX table of signal VC_PowerLimitValueAtFullThrottle in msg VC_PowerLimit.
 */
float app_canTx_VC_PowerLimit_VC_PowerLimitValueAtFullThrottle_get(void);
/**
 * Get value stored in TX table of signal VC_RegenEnabled in msg VC_VehicleDynamics.
 */
bool app_canTx_VC_VehicleDynamics_VC_RegenEnabled_get(void);
/**
 * Get value stored in TX table of signal VC_TorqueVectoringEnabled in msg VC_VehicleDynamics.
 */
bool app_canTx_VC_VehicleDynamics_VC_TorqueVectoringEnabled_get(void);
/**
 * Get value stored in TX table of signal VC_VehicleVelocity in msg VC_VehicleDynamics.
 */
float app_canTx_VC_VehicleDynamics_VC_VehicleVelocity_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityNorth in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityNorth_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityNorthAccuracy in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityNorthAccuracy_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityEast in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityEast_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityEastAccuracy in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityEastAccuracy_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityDown in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityDown_get(void);
/**
 * Get value stored in TX table of signal VC_VelocityDownAccuracy in msg VC_EllipseEkfNavVelocity.
 */
float app_canTx_VC_EllipseEkfNavVelocity_VC_VelocityDownAccuracy_get(void);
/**
 * Get value stored in TX table of signal VC_BatVoltage in msg VC_Voltages.
 */
float app_canTx_VC_Voltages_VC_BatVoltage_get(void);
/**
 * Get value stored in TX table of signal VC_BoostVoltage in msg VC_Voltages.
 */
float app_canTx_VC_Voltages_VC_BoostVoltage_get(void);
/**
 * Get value stored in TX table of signal VC_AccVoltage in msg VC_Voltages.
 */
float app_canTx_VC_Voltages_VC_AccVoltage_get(void);
/**
 * Get value stored in TX table of signal VC_AccCurrent in msg VC_CurrentSensing.
 */
float app_canTx_VC_CurrentSensing_VC_AccCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_BatCurrent in msg VC_CurrentSensing.
 */
float app_canTx_VC_CurrentSensing_VC_BatCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_ShdnCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_ShdnCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_ShdnStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_ShdnStatus_get(void);
/**
 * Get value stored in TX table of signal VC_LvCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_LvCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_LvStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_LvStatus_get(void);
/**
 * Get value stored in TX table of signal VC_PumpCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_PumpCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_PumpStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_PumpStatus_get(void);
/**
 * Get value stored in TX table of signal VC_AuxCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_AuxCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_AuxStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_AuxStatus_get(void);
/**
 * Get value stored in TX table of signal VC_InvRCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_InvRCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_InvRStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_InvRStatus_get(void);
/**
 * Get value stored in TX table of signal VC_InvLCurrent in msg VC_Efuse.
 */
float app_canTx_VC_Efuse_VC_InvLCurrent_get(void);
/**
 * Get value stored in TX table of signal VC_InvLStatus in msg VC_Efuse.
 */
bool app_canTx_VC_Efuse_VC_InvLStatus_get(void);
/**
 * Get value stored in TX table of signal VC_ImuAccelerationX in msg VC_ImuLinearAcceleration.
 */
float app_canTx_VC_ImuLinearAcceleration_VC_ImuAccelerationX_get(void);
/**
 * Get value stored in TX table of signal VC_ImuAccelerationY in msg VC_ImuLinearAcceleration.
 */
float app_canTx_VC_ImuLinearAcceleration_VC_ImuAccelerationY_get(void);
/**
 * Get value stored in TX table of signal VC_ImuAccelerationZ in msg VC_ImuLinearAcceleration.
 */
float app_canTx_VC_ImuLinearAcceleration_VC_ImuAccelerationZ_get(void);
/**
 * Get value stored in TX table of signal VC_FlowRate in msg VC_CoolantPumpFlowRate.
 */
float app_canTx_VC_CoolantPumpFlowRate_VC_FlowRate_get(void);
/**
 * Get value stored in TX table of signal VC_LEStopOKStatus in msg VC_VCShdnNodeStatus.
 */
bool app_canTx_VC_VCShdnNodeStatus_VC_LEStopOKStatus_get(void);
/**
 * Get value stored in TX table of signal VC_REStopOKStatus in msg VC_VCShdnNodeStatus.
 */
bool app_canTx_VC_VCShdnNodeStatus_VC_REStopOKStatus_get(void);
/**
 * Get value stored in TX table of signal VC_TSMSOKStatus in msg VC_VCShdnNodeStatus.
 */
bool app_canTx_VC_VCShdnNodeStatus_VC_TSMSOKStatus_get(void);
/**
 * Get value stored in TX table of signal VC_SplitterBoxInterlockOKStatus in msg VC_VCShdnNodeStatus.
 */
bool app_canTx_VC_VCShdnNodeStatus_VC_SplitterBoxInterlockOKStatus_get(void);
/**
 * Get value stored in TX table of signal VC_FirstFaultNode in msg VC_GlobalShdnNodeStatus.
 */
ShutdownNode app_canTx_VC_GlobalShdnNodeStatus_VC_FirstFaultNode_get(void);
/**
 * Get value stored in TX table of signal VC_MappedPedalPercentage in msg VC_PedalPercentage.
 */
float app_canTx_VC_PedalPercentage_VC_MappedPedalPercentage_get(void);


/**
 * Get pointer to value stored in TX table of msg VC_Vitals.
 */
VC_Vitals_Signals* app_canTx_VC_Vitals_getData();
/**
 * Get pointer to value stored in TX table of msg VC_LeftInverterCommand.
 */
VC_LeftInverterCommand_Signals* app_canTx_VC_LeftInverterCommand_getData();
/**
 * Get pointer to value stored in TX table of msg VC_INVL_ReadWriteParamCommand.
 */
VC_INVL_ReadWriteParamCommand_Signals* app_canTx_VC_INVL_ReadWriteParamCommand_getData();
/**
 * Get pointer to value stored in TX table of msg VC_RightInverterCommand.
 */
VC_RightInverterCommand_Signals* app_canTx_VC_RightInverterCommand_getData();
/**
 * Get pointer to value stored in TX table of msg VC_INVR_ReadWriteParamCommand.
 */
VC_INVR_ReadWriteParamCommand_Signals* app_canTx_VC_INVR_ReadWriteParamCommand_getData();
/**
 * Get pointer to value stored in TX table of msg VC_AlertsContext.
 */
VC_AlertsContext_Signals* app_canTx_VC_AlertsContext_getData();
/**
 * Get pointer to value stored in TX table of msg VC_EllipseStatus.
 */
VC_EllipseStatus_Signals* app_canTx_VC_EllipseStatus_getData();
/**
 * Get pointer to value stored in TX table of msg VC_EllipseTime.
 */
VC_EllipseTime_Signals* app_canTx_VC_EllipseTime_getData();
/**
 * Get pointer to value stored in TX table of msg VC_EllipseEulerAngles.
 */
VC_EllipseEulerAngles_Signals* app_canTx_VC_EllipseEulerAngles_getData();
/**
 * Get pointer to value stored in TX table of msg VC_CommitInfo.
 */
VC_CommitInfo_Signals* app_canTx_VC_CommitInfo_getData();
/**
 * Get pointer to value stored in TX table of msg VC_PowerEstimate.
 */
VC_PowerEstimate_Signals* app_canTx_VC_PowerEstimate_getData();
/**
 * Get pointer to value stored in TX table of msg VC_PowerEstimatePID.
 */
VC_PowerEstimatePID_Signals* app_canTx_VC_PowerEstimatePID_getData();
/**
 * Get pointer to value stored in TX table of msg VC_PIDSlipRatio.
 */
VC_PIDSlipRatio_Signals* app_canTx_VC_PIDSlipRatio_getData();
/**
 * Get pointer to value stored in TX table of msg VC_SlipRatios.
 */
VC_SlipRatios_Signals* app_canTx_VC_SlipRatios_getData();
/**
 * Get pointer to value stored in TX table of msg VC_ActiveDiffOutputs.
 */
VC_ActiveDiffOutputs_Signals* app_canTx_VC_ActiveDiffOutputs_getData();
/**
 * Get pointer to value stored in TX table of msg VC_PowerLimit.
 */
VC_PowerLimit_Signals* app_canTx_VC_PowerLimit_getData();
/**
 * Get pointer to value stored in TX table of msg VC_VehicleDynamics.
 */
VC_VehicleDynamics_Signals* app_canTx_VC_VehicleDynamics_getData();
/**
 * Get pointer to value stored in TX table of msg VC_EllipseEkfNavVelocity.
 */
VC_EllipseEkfNavVelocity_Signals* app_canTx_VC_EllipseEkfNavVelocity_getData();
/**
 * Get pointer to value stored in TX table of msg VC_Voltages.
 */
VC_Voltages_Signals* app_canTx_VC_Voltages_getData();
/**
 * Get pointer to value stored in TX table of msg VC_CurrentSensing.
 */
VC_CurrentSensing_Signals* app_canTx_VC_CurrentSensing_getData();
/**
 * Get pointer to value stored in TX table of msg VC_Efuse.
 */
VC_Efuse_Signals* app_canTx_VC_Efuse_getData();
/**
 * Get pointer to value stored in TX table of msg VC_ImuLinearAcceleration.
 */
VC_ImuLinearAcceleration_Signals* app_canTx_VC_ImuLinearAcceleration_getData();
/**
 * Get pointer to value stored in TX table of msg VC_CoolantPumpFlowRate.
 */
VC_CoolantPumpFlowRate_Signals* app_canTx_VC_CoolantPumpFlowRate_getData();
/**
 * Get pointer to value stored in TX table of msg VC_VCShdnNodeStatus.
 */
VC_VCShdnNodeStatus_Signals* app_canTx_VC_VCShdnNodeStatus_getData();
/**
 * Get pointer to value stored in TX table of msg VC_GlobalShdnNodeStatus.
 */
VC_GlobalShdnNodeStatus_Signals* app_canTx_VC_GlobalShdnNodeStatus_getData();
/**
 * Get pointer to value stored in TX table of msg VC_PedalPercentage.
 */
VC_PedalPercentage_Signals* app_canTx_VC_PedalPercentage_getData();




