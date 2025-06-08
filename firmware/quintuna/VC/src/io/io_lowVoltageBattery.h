#pragma once

#include "app_utils.h"
#include "io_lowVoltageBatReg.h"
#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#include "hw_i2cs.h"
#endif
typedef enum
{
    ACCUMULATED_CHARGE_COMMAND = 0x0076
} charge_cmd_t;

typedef enum
{
    BATTERY_STATUS = 0x12,
    CONTROL_STATUS = 0x00
} commands;
typedef enum
{
    CELL0_VOLTAGE_COMMAND = 0x14,
    CELL1_VOLTAGE_COMMAND = 0x16,
    CELL2_VOLTAGE_COMMAND = 0x1A,
    CELL3_VOLTAGE_COMMAND = 0x1C,
    STACK_VOLTAGE_COMMAND = 0x34
} voltage_cmd_t;
/**
 * @brief Initializes the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
ExitCode io_lowVoltageBattery_init();

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
double io_lowVoltageBattery_get_SOC();

/**
 * @brief Gets the battery voltage.
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
float io_lowVoltageBattery_get_voltage(voltage_cmd_t voltage_cell);

/**
 * @brief Handles releasing the semaphore after an interupt.
 *
 * @param GPIO_pin from the interupt.
 */
void io_lowVoltageBattery_completeAlert(uint16_t GPIO_pin);


/**
 * @brief Function to obtain the battery status from the chip
 *
 * @param bat_status The battery status returned from the chip
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowvoltageBattery_batteryStatus(Battery_Status *bat_status);


/**
 * @brief Function to obtain the safety status from the chip
 *
 * @param safetyA The safety status returned from the chip
 * @param safetyB The safety status returned from the chip
 * @param safetyC The safety status returned from the chip
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowVoltageBattery_SafetyStatusCheck(SafetyStatusA *safetyA, SafetyStatusB *safetyB, SafetyStatusC *safetyC);

/**
 * @brief Function to obtain the battery status from the chip
 *
 * @param ctrl_status The control status returned from the chip
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowVoltageBattery_controlStatus(Control_Status *ctrl_status);

/**
 * @brief Control the balancing of the batteries
 *
 * @param cell0 if high, enables balancing for this cell
 * @param cell1 if high, enables balancing for this cell
 * @param cell2 if high, enables balancing for this cell
 * @param cell3 if high, enables balancing for this cell
 * @return Exitcode to say whether transmission was successful
 */
ExitCode io_lowVoltageBattery_controlBalancing(bool cell0, bool cell1, bool cell2, bool cell3);

/**
 * @brief Configure the threshold (mV) for balancing, if a cell is 
 * above this threshold it will begin balancing
 *
 * @param voltage voltage threshold to balance at (mV)
 * @return Exitcode to say whether the transaction was succesful or not
 */
ExitCode io_lowVoltageBattery_configureBalancingThreshold(uint16_t voltage);

ExitCode io_lowVoltageBattery_showCellsBalancing(BalanceStatus *status);

// ExitCode io_lowVoltageBattery_additionalMeasurements(void);

// Cell Balancing Operation p77
// additionaBl measurements p22