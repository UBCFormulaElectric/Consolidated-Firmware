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
// clang format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_PROFET2(                                                   \
    index, efuse0_pin, efuse0_port, efuse0_ampere_per_volt, efuse1_pin, \
    efuse1_port, efuse1_ampere_per_volt, dsel_pin, dsel_port, den_pin,  \
    den_port)                                                           \
    [index].efuse[SENSE_0].input_channel.pin  = efuse0_pin,             \
    [index].efuse[SENSE_0].input_channel.port = efuse0_port,            \
    [index].efuse[SENSE_0].current            = 0,                      \
    [index].efuse[SENSE_0].ampere_per_volt    = efuse0_ampere_per_volt, \
    [index].efuse[SENSE_1].input_channel.pin  = efuse1_pin,             \
    [index].efuse[SENSE_1].input_channel.port = efuse1_port,            \
    [index].efuse[SENSE_1].current            = 0,                      \
    [index].efuse[SENSE_1].ampere_per_volt    = efuse1_ampere_per_volt, \
    [index].dsel_pin_mapping.pin              = dsel_pin,               \
    [index].dsel_pin_mapping.port             = dsel_port,              \
    [index].den_pin_mapping.pin               = den_pin,                \
    [index].den_pin_mapping.port              = den_port

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static Profet2_Struct profet2[NUM_PROFET2S] = {
    INIT_PROFET2(AUX1_AUX2, EFUSE_AUX_1_IN_Pin, EFUSE_AUX_1_IN_GPIO_Port,
                 AMP_PER_VOLT_AUX, EFUSE_AUX_2_IN_Pin, EFUSE_AUX_2_IN_GPIO_Port,
                 AMP_PER_VOLT_AUX, EFUSE_DSEL_1_Pin, EFUSE_DSEL_1_GPIO_Port,
                 EFUSE_DEN_1_Pin, EFUSE_DEN_1_GPIO_Port),
    INIT_PROFET2(COOLING_PDMFAN, EFUSE_COOLING_IN_Pin, EFUSE_COOLING_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_PDM_FAN_IN_Pin, EFUSE_PDM_FAN_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_DSEL_2_Pin, EFUSE_DSEL_2_GPIO_Port,
                 EFUSE_DEN_2_Pin, EFUSE_DEN_2_GPIO_Port),
    INIT_PROFET2(AIRSHDN_CANGLV, EFUSE_AIR_SHDN_IN_Pin, EFUSE_AIR_SHDN_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_CAN_IN_Pin, EFUSE_CAN_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_DSEL_3_Pin, EFUSE_DSEL_3_GPIO_Port,
                 EFUSE_DEN_3_Pin, EFUSE_DEN_3_GPIO_Port),
    INIT_PROFET2(ACCSEGMENTFAN_ACCENCLOSUREFAN, EFUSE_ACC_SEG_FAN_IN_Pin, EFUSE_ACC_SEG_FAN_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_ACC_ENC_FAN_IN_Pin, EFUSE_ACC_ENC_FAN_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_DSEL_4_Pin, EFUSE_DSEL_4_GPIO_Port,
                 EFUSE_DEN_4_Pin, EFUSE_DEN_4_GPIO_Port),
    INIT_PROFET2(LEFTINVERTER_RIGHTINVERTER, EFUSE_LEFT_INVERTER_IN_Pin, EFUSE_LEFT_INVERTER_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_RIGHT_INVERTER_IN_Pin, EFUSE_RIGHT_INVERTER_IN_GPIO_Port,
                 AMP_PER_VOLT, EFUSE_DSEL_5_Pin, EFUSE_DSEL_5_GPIO_Port,
                 EFUSE_DEN_5_Pin, EFUSE_DEN_5_GPIO_Port),
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

void GPIO_ConfigureFor12VAcc(volatile uint8_t *fault_states)
{
    // TODO: If an efuse has exceeded its max retry count, don't turn it on
    for (uint32_t i = 0; i < NUM_PROFET2S; i++)
    {
        Profet2_Struct *profet2 = &Gpio_GetProfet2s()[i];
        SharedGpio_GPIO_WritePin(
            profet2->den_pin_mapping.port, profet2->den_pin_mapping.pin,
            DEN_ON);

        for (uint32_t j = 0; j < NUM_CHANNELS_PER_PROFET2; j++)
        {
            Gpio_ConfigureSingleEfuse(
                &(Gpio_GetProfet2s()[i].efuse[j]), EFUSE_ON);
        }
    }
}

void GPIO_ConfigureFor12VAux(volatile uint8_t *fault_states)
{
    Profet2_Struct *profet2s = Gpio_GetProfet2s();

    // Turn off everything except for AIR shutdown and CAN GLV
    for (uint32_t i = 0; i < NUM_PROFET2S; i++)
    {
        if (&profet2s[i] == &profet2s[AIRSHDN_CANGLV])
        {
            // TODO: If an efuse has exceeded its max retry count, don't turn it
            // on
            Gpio_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_0]), EFUSE_ON);
            Gpio_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_1]), EFUSE_ON);
            Gpio_ConfigureSingleDen(&profet2s[i], DEN_ON);
        }
        else
        {
            Gpio_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_0]), EFUSE_OFF);
            Gpio_ConfigureSingleEfuse(&(profet2s[i].efuse[SENSE_1]), EFUSE_OFF);
            Gpio_ConfigureSingleDen(&profet2s[i], DEN_OFF);
        }
    }
}

void Gpio_ConfigureSingleEfuse(
    efuse_struct *           efuse,
    EfuseOnOff_GPIO_PinState state)
{
    SharedGpio_GPIO_WritePin(
        efuse->input_channel.port, efuse->input_channel.pin, state);
}

void Gpio_ConfigureSingleDen(
    Profet2_Struct *       profet2,
    DenOnOff_GPIO_PinState state)
{
    SharedGpio_GPIO_WritePin(
        profet2->den_pin_mapping.port, profet2->den_pin_mapping.pin, state);
}

void Gpio_ConfigureAllDsels(DselOnOff_GPIO_PinState state)
{
    // TODO: Test that the sizeof works and this function works in general
    for (Profet2Index_Enum i = 0; i < sizeof(profet2) / sizeof(profet2[0]); i++)
    {
        SharedGpio_GPIO_WritePin(
            profet2[i].dsel_pin_mapping.port, profet2[i].dsel_pin_mapping.pin,
            state);
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

Profet2_Struct *const Gpio_GetProfet2s(void)
{
    return (Profet2_Struct *const)(&profet2[0]);
}
