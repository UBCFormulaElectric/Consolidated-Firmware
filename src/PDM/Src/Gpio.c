/******************************************************************************
* Includes
*******************************************************************************/
#include "Gpio.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void GPIO_Init(void) {
    // Start DSELs at output 0
    GPIO_EFuseSelectDSEL(dsel_state);

    // Check for faults on startup
    GPIO_CheckFaultsStartup();
}

void GPIO_ConfigurePreChargeComplete(volatile uint8_t* fault_states) {
	
	// E-Fuse AUX 1/2
	if(fault_states[AUX_1_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_AUX_1_IN_GPIO_Port, EFUSE_AUX_1_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[AUX_2_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_AUX_2_IN_GPIO_Port, EFUSE_AUX_2_IN_Pin, GPIO_PIN_SET);
	}
    HAL_GPIO_WritePin(EFUSE_AUX_DEN_GPIO_Port, EFUSE_AUX_DEN_Pin, GPIO_PIN_SET);

    // E-Fuse PDM Fan/Cooling
	if(fault_states[PDM_FAN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(
		EFUSE_PDM_FAN_IN_GPIO_Port, EFUSE_PDM_FAN_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[COOLING_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(
		EFUSE_COOLING_IN_GPIO_Port, EFUSE_COOLING_IN_Pin, GPIO_PIN_SET);
	}
    HAL_GPIO_WritePin(
    EFUSE_FAN_COOLING_DEN_GPIO_Port, EFUSE_FAN_COOLING_DEN_Pin, GPIO_PIN_SET);

    // E-Fuse CAN/AIR SHDN
	if(fault_states[CAN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_CAN_IN_GPIO_Port, EFUSE_CAN_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[AIR_SHDN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_AIR_SHDN_IN_Pin, GPIO_PIN_SET);
	}
    HAL_GPIO_WritePin(
    EFUSE_CAN_AIR_SHDN_DEN_GPIO_Port, EFUSE_CAN_AIR_SHDN_DEN_Pin, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
	if(fault_states[ACC_SEG_FAN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_ACC_SEG_FAN_IN_GPIO_Port, EFUSE_ACC_SEG_FAN_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[ACC_ENC_FAN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_ACC_ENC_FAN_IN_GPIO_Port, EFUSE_ACC_ENC_FAN_IN_Pin, GPIO_PIN_SET);
	}
    HAL_GPIO_WritePin(
    EFUSE_ACC_FAN_DEN_GPIO_Port, EFUSE_ACC_FAN_DEN_Pin, GPIO_PIN_SET);

    // E-Fuse Inverter
	if(fault_states[L_INV_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_LEFT_INVERTER_IN_GPIO_Port, EFUSE_LEFT_INVERTER_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[R_INV_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_RIGHT_INVERTER_IN_GPIO_Port, EFUSE_RIGHT_INVERTER_IN_Pin, GPIO_PIN_SET);
	}
		
    HAL_GPIO_WritePin(
    EFUSE_INVERTER_DEN_GPIO_Port, EFUSE_INVERTER_DEN_Pin, GPIO_PIN_SET);
}
void GPIO_ConfigurePowerUp(volatile uint8_t* fault_states) {
    // E-Fuse AUX 1/2
    HAL_GPIO_WritePin(EFUSE_AUX_1_IN_GPIO_Port, EFUSE_AUX_1_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EFUSE_AUX_2_IN_GPIO_Port, EFUSE_AUX_2_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EFUSE_AUX_DEN_GPIO_Port, EFUSE_AUX_DEN_Pin, GPIO_PIN_RESET);

    // E-Fuse PDM Fan/Cooling
    HAL_GPIO_WritePin(
    EFUSE_PDM_FAN_IN_GPIO_Port, EFUSE_PDM_FAN_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_COOLING_IN_GPIO_Port, EFUSE_COOLING_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_FAN_COOLING_DEN_GPIO_Port, EFUSE_FAN_COOLING_DEN_Pin, GPIO_PIN_RESET);

	// E-Fuse CAN/AIR SHDN
	if(fault_states[CAN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_CAN_IN_GPIO_Port, EFUSE_CAN_IN_Pin, GPIO_PIN_SET);
	}
	if(fault_states[AIR_SHDN_INDEX] == STATIC_EFUSE){
		HAL_GPIO_WritePin(EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_AIR_SHDN_IN_Pin, GPIO_PIN_SET);
	}
    HAL_GPIO_WritePin(
    EFUSE_CAN_AIR_SHDN_DEN_GPIO_Port, EFUSE_CAN_AIR_SHDN_DEN_Pin, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
    HAL_GPIO_WritePin(
    EFUSE_ACC_SEG_FAN_IN_GPIO_Port, EFUSE_ACC_SEG_FAN_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_ACC_ENC_FAN_IN_GPIO_Port, EFUSE_ACC_ENC_FAN_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_ACC_FAN_DEN_GPIO_Port, EFUSE_ACC_FAN_DEN_Pin, GPIO_PIN_RESET);

    // E-Fuse Inverter
    HAL_GPIO_WritePin(
    EFUSE_INVERTER_DEN_GPIO_Port, EFUSE_INVERTER_DEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_LEFT_INVERTER_IN_GPIO_Port, EFUSE_LEFT_INVERTER_IN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_RIGHT_INVERTER_IN_GPIO_Port, EFUSE_RIGHT_INVERTER_IN_Pin, GPIO_PIN_RESET);
}

void GPIO_EFuseSelectDSEL(GPIO_PinState dsel_value) {
    HAL_GPIO_WritePin(EFUSE_AUX_DSEL_GPIO_Port, EFUSE_AUX_DSEL_Pin, dsel_value);
    HAL_GPIO_WritePin(
    EFUSE_FAN_COOLING_DSEL_GPIO_Port, EFUSE_FAN_COOLING_DSEL_Pin, dsel_value);
    HAL_GPIO_WritePin(
    EFUSE_CAN_AIR_SHDN_DSEL_GPIO_Port, EFUSE_CAN_AIR_SHDN_DSEL_Pin, dsel_value);
    HAL_GPIO_WritePin(EFUSE_ACC_FAN_DSEL_GPIO_Port, EFUSE_ACC_FAN_DSEL_Pin, dsel_value);
    HAL_GPIO_WritePin(
    EFUSE_INVERTER_DSEL_GPIO_Port, EFUSE_INVERTER_DSEL_Pin, dsel_value);
}

void GPIO_HAL_GPIO_EXTI_Callback(uint16_t gpio_pin) {
    uint32_t can_error_msg = 0;
    switch (gpio_pin) {
        case CHARGER_FAULT_Pin:
            if (HAL_GPIO_ReadPin(CHARGER_FAULT_GPIO_Port, CHARGER_FAULT_Pin) ==
                CHARGER_FAULT_STATE) {
                TransmitCANError(PDM_ERROR,
                                 Power_Distribution_Module,
                                 CHARGER_FAULT,
                                 can_error_msg);
            }
            break;
        case CELL_BALANCE_OVERVOLTAGE_Pin:
            if (HAL_GPIO_ReadPin(CELL_BALANCE_OVERVOLTAGE_GPIO_Port,
                                 CELL_BALANCE_OVERVOLTAGE_Pin) ==
                CELL_BALANCE_OVERVOLTAGE_FAULT_STATE) {
                can_error_msg = (16 << 8);
                TransmitCANError(PDM_ERROR,
                                 Power_Distribution_Module,
                                 CELL_BALANCE_OVERVOLTAGE_FAULT,
                                 can_error_msg);
            }
            break;
        case BOOST_PGOOD_Pin:
            if (HAL_GPIO_ReadPin(BOOST_PGOOD_GPIO_Port, BOOST_PGOOD_Pin) ==
                BOOST_PGOOD_FAULT_STATE) {
                can_error_msg = (17 << 8);
                TransmitCANError(PDM_ERROR,
                                 Power_Distribution_Module,
                                 BOOST_PGOOD_FAULT,
                                 can_error_msg);
            }
            break;
    }
}


void GPIO_CheckFaultsStartup(void) {
    uint32_t can_error_msg = 0;
    // Check for charger fault
    if (HAL_GPIO_ReadPin(CHARGER_FAULT_GPIO_Port, CHARGER_FAULT_Pin) ==
        CHARGER_FAULT_STATE) {
        TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, CHARGER_FAULT, can_error_msg);
    }
    // Check for charger charging
    if (HAL_GPIO_ReadPin(CHARGER_INDICATOR_GPIO_Port, CHARGER_INDICATOR_Pin) ==
        CHARGER_CHARGING_STATE) {
        //  Currently not transmitting the charger 'charging' state - usually
        //  constantly true and not an 'error'
        //	TransmitCANError(General_Error_StandardID,
        //Power_Distribution_Module, PDM_Misc_Error, can_error_msg);
    }
    // Check for overvoltage fault
    if (HAL_GPIO_ReadPin(CELL_BALANCE_OVERVOLTAGE_GPIO_Port,
                         CELL_BALANCE_OVERVOLTAGE_Pin) ==
        CELL_BALANCE_OVERVOLTAGE_FAULT_STATE) {
        TransmitCANError(PDM_ERROR,
                         Power_Distribution_Module,
                         CELL_BALANCE_OVERVOLTAGE_FAULT,
                         can_error_msg);
    }
    // Check for boost converter fault
    if (HAL_GPIO_ReadPin(BOOST_PGOOD_GPIO_Port, BOOST_PGOOD_Pin) ==
        BOOST_PGOOD_FAULT_STATE) {
        TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, BOOST_PGOOD_FAULT, can_error_msg);
    }
}
