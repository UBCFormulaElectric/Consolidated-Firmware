/******************************************************************************
 * Includes
 *******************************************************************************/
#include "Gpio.h"
#include "stdbool.h"

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
    return HAL_GPIO_ReadPin(BOOST_PGOOD_PORT, BOOST_PGOOD_PIN) ==
           BOOST_PGOOD_FAULT_STATE;
}

static bool GPIO_IsCellBalanceOvervoltageFaultActive(void)
{
    return HAL_GPIO_ReadPin(
               CELL_BALANCE_OVERVOLTAGE_PORT, CELL_BALANCE_OVERVOLTAGE_PIN) ==
           CELL_BALANCE_OVERVOLTAGE_FAULT_STATE;
}

static bool GPIO_IsChargingActive(void)
{
    return HAL_GPIO_ReadPin(CHARGER_PORT, CHARGER_INDICATOR_PIN) ==
           CHARGER_CHARGING_STATE;
}

static bool GPIO_IsChargerFaultActive(void)
{
    return HAL_GPIO_ReadPin(CHARGER_PORT, CHARGER_FAULT_PIN) ==
           CHARGER_FAULT_STATE;
}

static void GPIO_BoostPgoodFaultHandler(void)
{
    if (GPIO_IsBoostPgoodFaultActive())
    {
        Can_BroadcastPdmErrors(BOOST_PGOOD_FAULT);
    }
}

static void GPIO_CellBalanceOvervoltageFaultHandler(void)
{
    if (GPIO_IsCellBalanceOvervoltageFaultActive())
    {
        Can_BroadcastPdmErrors(CELL_BALANCE_OVERVOLTAGE_FAULT);
    }
}

static void GPIO_ChargerFaultHandler(void)
{
    if (GPIO_IsChargerFaultActive())
    {
        Can_BroadcastPdmErrors(CHARGER_FAULT);
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
 *******************************************************************************/
void GPIO_Init(void)
{
    // Start DSELs at output 0
    GPIO_EFuseSelectDSEL(DSEL_LOW);

    // Check for faults on startup
    GPIO_CheckFaultsStartup();
}

void GPIO_ConfigurePreChargeComplete(volatile uint8_t *fault_states)
{
    // E-Fuse AUX 1/2
    if (fault_states[AUXILIARY_1] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_AUX_1_IN_PORT, EFUSE_AUX_1_IN_PIN, GPIO_PIN_SET);
    }
    if (fault_states[AUXILIARY_2] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_AUX_2_IN_PORT, EFUSE_AUX_2_IN_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(EFUSE_AUX_DEN_PORT, EFUSE_AUX_DEN_PIN, GPIO_PIN_SET);

    // E-Fuse PDM Fan/Cooling
    if (fault_states[PDM_FAN] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_PDM_FAN_IN_PORT, EFUSE_PDM_FAN_IN_PIN, GPIO_PIN_SET);
    }
    if (fault_states[COOLING] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_COOLING_IN_PORT, EFUSE_COOLING_IN_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(
        EFUSE_FAN_COOLING_DEN_PORT, EFUSE_FAN_COOLING_DEN_PIN, GPIO_PIN_SET);

    // E-Fuse CAN_GLV/AIR SHDN
    if (fault_states[CAN_GLV] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(EFUSE_CAN_IN_PORT, EFUSE_CAN_IN_PIN, GPIO_PIN_SET);
    }
    if (fault_states[AIR_SHDN] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_AIR_SHDN_IN_PORT, EFUSE_AIR_SHDN_IN_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(
        EFUSE_CAN_AIR_SHDN_DEN_PORT, EFUSE_CAN_AIR_SHDN_DEN_PIN, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
    if (fault_states[ACC_SEGMENT_FAN] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_ACC_SEG_FAN_IN_PORT, EFUSE_ACC_SEG_FAN_IN_PIN, GPIO_PIN_SET);
    }
    if (fault_states[ACC_ENCLOSURE_FAN] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_ACC_ENC_FAN_IN_PORT, EFUSE_ACC_ENC_FAN_IN_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(
        EFUSE_ACC_FAN_DEN_PORT, EFUSE_ACC_FAN_DEN_PIN, GPIO_PIN_SET);

    // E-Fuse Inverter
    if (fault_states[LEFT_INVERTER] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_LEFT_INVERTER_IN_PORT, EFUSE_LEFT_INVERTER_IN_PIN,
            GPIO_PIN_SET);
    }
    if (fault_states[RIGHT_INVERTER] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_RIGHT_INVERTER_IN_PORT, EFUSE_RIGHT_INVERTER_IN_PIN,
            GPIO_PIN_SET);
    }

    HAL_GPIO_WritePin(
        EFUSE_INVERTER_DEN_PORT, EFUSE_INVERTER_DEN_PIN, GPIO_PIN_SET);
}
void GPIO_ConfigurePowerUp(volatile uint8_t *fault_states)
{
    // E-Fuse AUX 1/2
    HAL_GPIO_WritePin(EFUSE_AUX_1_IN_PORT, EFUSE_AUX_1_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EFUSE_AUX_2_IN_PORT, EFUSE_AUX_2_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EFUSE_AUX_DEN_PORT, EFUSE_AUX_DEN_PIN, GPIO_PIN_RESET);

    // E-Fuse PDM Fan/Cooling
    HAL_GPIO_WritePin(
        EFUSE_PDM_FAN_IN_PORT, EFUSE_PDM_FAN_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_COOLING_IN_PORT, EFUSE_COOLING_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_FAN_COOLING_DEN_PORT, EFUSE_FAN_COOLING_DEN_PIN, GPIO_PIN_RESET);

    // E-Fuse CAN_GLV/AIR SHDN
    if (fault_states[CAN_GLV] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(EFUSE_CAN_IN_PORT, EFUSE_CAN_IN_PIN, GPIO_PIN_SET);
    }
    if (fault_states[AIR_SHDN] == NORMAL_STATE)
    {
        HAL_GPIO_WritePin(
            EFUSE_AIR_SHDN_IN_PORT, EFUSE_AIR_SHDN_IN_PIN, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(
        EFUSE_CAN_AIR_SHDN_DEN_PORT, EFUSE_CAN_AIR_SHDN_DEN_PIN, GPIO_PIN_SET);

    // E-Fuse Accumulator Fans
    HAL_GPIO_WritePin(
        EFUSE_ACC_SEG_FAN_IN_PORT, EFUSE_ACC_SEG_FAN_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_ACC_ENC_FAN_IN_PORT, EFUSE_ACC_ENC_FAN_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_ACC_FAN_DEN_PORT, EFUSE_ACC_FAN_DEN_PIN, GPIO_PIN_RESET);

    // E-Fuse Inverter
    HAL_GPIO_WritePin(
        EFUSE_INVERTER_DEN_PORT, EFUSE_INVERTER_DEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_LEFT_INVERTER_IN_PORT, EFUSE_LEFT_INVERTER_IN_PIN,
        GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
        EFUSE_RIGHT_INVERTER_IN_PORT, EFUSE_RIGHT_INVERTER_IN_PIN,
        GPIO_PIN_RESET);
}

void GPIO_EFuseSelectDSEL(GPIO_PinState dsel_value)
{
    HAL_GPIO_WritePin(EFUSE_AUX_DSEL_PORT, EFUSE_AUX_DSEL_PIN, dsel_value);
    HAL_GPIO_WritePin(
        EFUSE_FAN_COOLING_DSEL_PORT, EFUSE_FAN_COOLING_DSEL_PIN, dsel_value);
    HAL_GPIO_WritePin(
        EFUSE_CAN_AIR_SHDN_DSEL_PORT, EFUSE_CAN_AIR_SHDN_DSEL_PIN, dsel_value);
    HAL_GPIO_WritePin(
        EFUSE_ACC_FAN_DSEL_PORT, EFUSE_ACC_FAN_DSEL_PIN, dsel_value);
    HAL_GPIO_WritePin(
        EFUSE_INVERTER_DSEL_PORT, EFUSE_INVERTER_DSEL_PIN, dsel_value);
}

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    switch (gpio_pin)
    {
        case CHARGER_FAULT_PIN:
            GPIO_ChargerFaultHandler();
            break;
        case CELL_BALANCE_OVERVOLTAGE_PIN:
            GPIO_CellBalanceOvervoltageFaultHandler();
            break;
        case BOOST_PGOOD_PIN:
            GPIO_BoostPgoodFaultHandler();
            break;
        default:
            break;
    }
}
