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
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

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

void GPIO_ConfigureFor12VAcc(void)
{
    // TODO: If an efuse has exceeded its max retry count, don't turn it on
    for (uint32_t i = 0; i < NUM_PROFET2S; i++)
    {
        Profet2_Struct *profet2 = &Profet2_GetProfet2s()[i];
        SharedGpio_GPIO_WritePin(
            profet2->den_pin_mapping.port, profet2->den_pin_mapping.pin,
            DEN_ON);

        for (uint32_t j = 0; j < NUM_CHANNELS_PER_PROFET2; j++)
        {
            Profet2_ConfigureSingleEfuse(
                &(Profet2_GetProfet2s()[i].efuse[j]), EFUSE_ON);
        }
    }
}

void GPIO_ConfigureFor12VAux(void)
{
    Profet2_Struct *profet2s = Profet2_GetProfet2s();

    // Turn off everything except for AIR shutdown and CAN GLV
    for (uint32_t i = 0; i < NUM_PROFET2S; i++)
    {
        if (&profet2s[i] == &profet2s[AIRSHDN_CANGLV])
        {
            // TODO: If an efuse has exceeded its max retry count, don't turn it
            // on
            Profet2_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_0]), EFUSE_ON);
            Profet2_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_1]), EFUSE_ON);
            Profet2_ConfigureSingleDen(&profet2s[i], DEN_ON);
        }
        else
        {
            Profet2_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_0]), EFUSE_OFF);
            Profet2_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_1]), EFUSE_OFF);
            Profet2_ConfigureSingleDen(&profet2s[i], DEN_OFF);
        }
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

