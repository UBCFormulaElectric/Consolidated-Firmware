/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Gpio.h"
#include "stdbool.h"
#include "CurrentSense.h"
#include "VoltageSense.h"
#include "SharedGpio.h"
#include "SharedCan.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_EFUSE(index, efuse_pin, efuse_port) \
        [index].pin = efuse_pin, [index].port = efuse_port,
#define INIT_DSEL(dsel_pin, dsel_port) {.pin = dsel_pin, .port = dsel_port},

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// The index can be any value of EfuseCurrentIndex_Enum
const GPIO_PinPort_Struct efuse_pin_mapping[NUM_EFUSES] = 
{
    INIT_EFUSE(AUXILIARY_1, EFUSE_AUX_1_IN_Pin, EFUSE_AUX_1_IN_GPIO_Port)
    INIT_EFUSE(COOLING, EFUSE_COOLING_IN_Pin, EFUSE_COOLING_IN_GPIO_Port)
    INIT_EFUSE(AIR_SHDN, EFUSE_AIR_SHDN_IN_Pin, EFUSE_AIR_SHDN_IN_GPIO_Port)
    INIT_EFUSE(ACC_SEGMENT_FAN, EFUSE_ACC_SEG_FAN_IN_Pin, EFUSE_ACC_SEG_FAN_IN_GPIO_Port)
    INIT_EFUSE(LEFT_INVERTER, EFUSE_LEFT_INVERTER_IN_Pin, EFUSE_LEFT_INVERTER_IN_GPIO_Port)
    INIT_EFUSE(AUXILIARY_2, EFUSE_AUX_2_IN_Pin, EFUSE_AUX_2_IN_GPIO_Port)
    INIT_EFUSE(PDM_FAN, EFUSE_PDM_FAN_IN_Pin, EFUSE_PDM_FAN_IN_GPIO_Port)
    INIT_EFUSE(CAN_GLV, EFUSE_CAN_IN_Pin, EFUSE_CAN_IN_GPIO_Port)
    INIT_EFUSE(ACC_ENCLOSURE_FAN, EFUSE_ACC_ENC_FAN_IN_Pin, EFUSE_ACC_ENC_FAN_IN_GPIO_Port)
    INIT_EFUSE(RIGHT_INVERTER, EFUSE_RIGHT_INVERTER_IN_Pin, EFUSE_RIGHT_INVERTER_IN_GPIO_Port)
};

const GPIO_PinPort_Struct dsel_pin_mapping[NUM_PROFET2S] =
{
    INIT_DSEL(EFUSE_DSEL_1_Pin, EFUSE_DSEL_1_GPIO_Port)
    INIT_DSEL(EFUSE_DSEL_2_Pin, EFUSE_DSEL_2_GPIO_Port)
    INIT_DSEL(EFUSE_DSEL_3_Pin, EFUSE_DSEL_3_GPIO_Port)
    INIT_DSEL(EFUSE_DSEL_4_Pin, EFUSE_DSEL_4_GPIO_Port)
    INIT_DSEL(EFUSE_DSEL_5_Pin, EFUSE_DSEL_5_GPIO_Port)
};

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/**
 * @brief  Check if the PGOOD fault condition of boost controller (LTC3786)
 *         is active
 * @return true Active fault
 *         false No Fault
 */
static bool GPIO_IsBoostPgoodFaultActive(void);

/**
 * @brief  Check if the overvoltage fault of cell balance IC (BQ29209) is active
 * @return true Active fault
 *         false No fault
 */
static bool GPIO_IsCellBalanceOvervoltageFaultActive(void);

/**
 * @brief  Check if the charging process of Li-Ion battery charger (LT3650-8.4)
 *         is active
 * @return true Active charging
 *         false No charging
 */
static bool GPIO_IsChargingActive(void);
/**
 * @brief  Check if the fault condition Li-Ion battery charger (LT3650-8.4)
 *         is active
 * @return true Active fault
 *         false No fault
 */
static bool GPIO_IsChargerFaultActive(void);

/**
 * @brief Check if the PGOOD fault of boost controller (LTC3786) is active and
 *        broadcast error over CAN if needed
 */
static void GPIO_BoostPgoodFaultHandler(void);

/**
 * @brief Check if the overvoltage fault of cell balance IC (BQ29209)
 *        and broadcast error over CAN if needed
 */
static void GPIO_CellBalanceOvervoltageFaultHandler(void);

/**
 * @brief Check if the fault condition of Li-Ion battery charger (LTC3650-8.4)
 *        is active and broadcast error over CAN if needed
 *
 */
static void GPIO_ChargerFaultHandler(void);

/**
 * @brief Check if the charging process of Li-Ion battery charger (LTC3650-8.4)
 *        is active (Currently doesn nothing if detected)
 */
static void GPIO_ChargingActiveHandler(void);
/**
 * @brief Some faults rely on EXTI callback catching edge-triggered interrupts
 *        to activate the appropriate handler. But if something was faulted from
 *        before system boot-up, then EXTI wouldn't catch the faults. Thus, we
 *        need to manually check for faults during system boot-up.
 *
 *        Faults being checked: charging IC, cell balancing IC, and boost
 *        converter. Detected faults will be broadcasted over CAN.
 */
static void GPIO_CheckFaultsStartup(void);

/******************************************************************************
 * Private Function Definitions
 *******************************************************************************/
static bool GPIO_IsBoostPgoodFaultActive(void)
{
    return HAL_GPIO_ReadPin(BOOST_PGOOD_GPIO_Port, BOOST_PGOOD_Pin) ==
           BOOST_PGOOD_FAULT_STATE;
}

static bool GPIO_IsCellBalanceOvervoltageFaultActive(void)
{
    return HAL_GPIO_ReadPin(
               CELL_BALANCE_OVERVOLTAGE_GPIO_Port,
               CELL_BALANCE_OVERVOLTAGE_Pin) ==
           CELL_BALANCE_OVERVOLTAGE_FAULT_STATE;
}

static bool GPIO_IsChargingActive(void)
{
    return HAL_GPIO_ReadPin(
               CHARGER_INDICATOR_GPIO_Port, CHARGER_INDICATOR_Pin) ==
           CHARGER_CHARGING_STATE;
}

static bool GPIO_IsChargerFaultActive(void)
{
    return HAL_GPIO_ReadPin(CHARGER_FAULT_GPIO_Port, CHARGER_FAULT_Pin) ==
           CHARGER_FAULT_STATE;
}

static void GPIO_BoostPgoodFaultHandler(void)
{
    if (GPIO_IsBoostPgoodFaultActive())
    {
        SharedCan_BroadcastPcbErrors(BOOST_PGOOD_FAULT);
    }
}

static void GPIO_CellBalanceOvervoltageFaultHandler(void)
{
    if (GPIO_IsCellBalanceOvervoltageFaultActive())
    {
        SharedCan_BroadcastPcbErrors(CELL_BALANCE_OVERVOLTAGE_FAULT);
    }
}

static void GPIO_ChargerFaultHandler(void)
{
    if (GPIO_IsChargerFaultActive())
    {
        SharedCan_BroadcastPcbErrors(CHARGER_FAULT);
    }
}

static void GPIO_ChargingActiveHandler(void)
{
    if (GPIO_IsChargingActive())
    {
        // Currently not transmitting the charger 'charging' state - usually
        // constantly true and not an 'error'. Evaluate if this is an error
        // at all
    }
}

static void GPIO_CheckFaultsStartup(void)
{
    GPIO_BoostPgoodFaultHandler();
    GPIO_CellBalanceOvervoltageFaultHandler();
    GPIO_ChargerFaultHandler();
    GPIO_ChargingActiveHandler();
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void GPIO_Init(void)
{
    // Start DSELs at output 0
    CurrentSense_SelectCurrentSenseChannel(SENSE_0);

    // Check for faults on startup
    GPIO_CheckFaultsStartup();
}

void GPIO_ConfigurePreChargeComplete(volatile uint8_t *fault_states)
{
    // E-Fuse AUX 1/2
    if (fault_states[AUXILIARY_1] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_AUX_1_IN_GPIO_Port, EFUSE_AUX_1_IN_Pin, GPIO_PIN_SET);
    }
    if (fault_states[AUXILIARY_2] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_AUX_2_IN_GPIO_Port, EFUSE_AUX_2_IN_Pin, GPIO_PIN_SET);
    }
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_1_GPIO_Port, EFUSE_DEN_1_Pin, GPIO_PIN_SET);

    // E-Fuse PDM Fan/Cooling
    if (fault_states[PDM_FAN] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_PDM_FAN_IN_GPIO_Port, EFUSE_PDM_FAN_IN_Pin, GPIO_PIN_SET);
    }
    if (fault_states[COOLING] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_COOLING_IN_GPIO_Port, EFUSE_COOLING_IN_Pin, GPIO_PIN_SET);
    }
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_2_GPIO_Port, EFUSE_DEN_2_Pin, GPIO_PIN_SET);

    // E-Fuse CAN_GLV/AIR SHDN
    if (fault_states[CAN_GLV] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_CAN_IN_GPIO_Port, EFUSE_CAN_IN_Pin, GPIO_PIN_SET);
    }
    if (fault_states[AIR_SHDN] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_AIR_SHDN_IN_Pin, GPIO_PIN_SET);
    }
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_3_GPIO_Port, EFUSE_DEN_3_Pin, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
    if (fault_states[ACC_SEGMENT_FAN] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_ACC_SEG_FAN_IN_GPIO_Port, EFUSE_ACC_SEG_FAN_IN_Pin,
            GPIO_PIN_SET);
    }
    if (fault_states[ACC_ENCLOSURE_FAN] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_ACC_ENC_FAN_IN_GPIO_Port, EFUSE_ACC_ENC_FAN_IN_Pin,
            GPIO_PIN_SET);
    }
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_4_GPIO_Port, EFUSE_DEN_4_Pin, GPIO_PIN_SET);

    // E-Fuse Inverter
    if (fault_states[LEFT_INVERTER] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_LEFT_INVERTER_IN_GPIO_Port, EFUSE_LEFT_INVERTER_IN_Pin,
            GPIO_PIN_SET);
    }
    if (fault_states[RIGHT_INVERTER] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_RIGHT_INVERTER_IN_GPIO_Port, EFUSE_RIGHT_INVERTER_IN_Pin,
            GPIO_PIN_SET);
    }

    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_5_GPIO_Port, EFUSE_DEN_5_Pin, GPIO_PIN_SET);
}

void GPIO_ConfigurePowerUp(volatile uint8_t *fault_states)
{
    // E-Fuse AUX 1/2
    SharedGpio_GPIO_WritePin(
        EFUSE_AUX_1_IN_GPIO_Port, EFUSE_AUX_1_IN_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_AUX_2_IN_GPIO_Port, EFUSE_AUX_2_IN_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_1_GPIO_Port, EFUSE_DEN_1_Pin, GPIO_PIN_RESET);

    // E-Fuse PDM Fan/Cooling
    SharedGpio_GPIO_WritePin(
        EFUSE_PDM_FAN_IN_GPIO_Port, EFUSE_PDM_FAN_IN_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_COOLING_IN_GPIO_Port, EFUSE_COOLING_IN_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_2_GPIO_Port, EFUSE_DEN_2_Pin, GPIO_PIN_RESET);

    // E-Fuse CAN/AIR SHDN
    if (fault_states[CAN_GLV] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_CAN_IN_GPIO_Port, EFUSE_CAN_IN_Pin, GPIO_PIN_SET);
    }
    if (fault_states[AIR_SHDN] == NORMAL_STATE)
    {
        SharedGpio_GPIO_WritePin(
            EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_AIR_SHDN_IN_Pin, GPIO_PIN_SET);
    }
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_3_GPIO_Port, EFUSE_DEN_3_Pin, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
    SharedGpio_GPIO_WritePin(
        EFUSE_ACC_SEG_FAN_IN_GPIO_Port, EFUSE_ACC_SEG_FAN_IN_Pin,
        GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_ACC_ENC_FAN_IN_GPIO_Port, EFUSE_ACC_ENC_FAN_IN_Pin,
        GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_4_GPIO_Port, EFUSE_DEN_4_Pin, GPIO_PIN_RESET);

    // E-Fuse Inverter
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_5_GPIO_Port, EFUSE_DEN_5_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_LEFT_INVERTER_IN_GPIO_Port, EFUSE_LEFT_INVERTER_IN_Pin,
        GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_RIGHT_INVERTER_IN_GPIO_Port, EFUSE_RIGHT_INVERTER_IN_Pin,
        GPIO_PIN_RESET);
}

void Gpio_ConfigureSingleEfuse(EfuseCurrentIndex_Enum index, EfuseOnOff_GPIO_PinState state)
{
   SharedGpio_GPIO_WritePin(efuse_pin_mapping[index].port,
                            efuse_pin_mapping[index].pin,
                            state);
}

void Gpio_ConfigureAllDsels(DselState_Enum state)
{
    for (uint32_t i = 0; i < sizeof(dsel_pin_mapping) / sizeof(dsel_pin_mapping[0]); i++)
    {
        SharedGpio_GPIO_WritePin(dsel_pin_mapping[i].port, dsel_pin_mapping[i].pin, state);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    switch (gpio_pin)
    {
        case CHARGER_FAULT_Pin:
            GPIO_ChargerFaultHandler();
            break;
        case CELL_BALANCE_OVERVOLTAGE_Pin:
            GPIO_CellBalanceOvervoltageFaultHandler();
            break;
        case BOOST_PGOOD_Pin:
            GPIO_BoostPgoodFaultHandler();
            break;
        default:
            break;
    }
}
