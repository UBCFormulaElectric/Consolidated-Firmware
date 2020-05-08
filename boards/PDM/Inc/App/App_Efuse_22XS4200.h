#pragma once
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "Io_Efuse_22XS4200.h"

struct E_Fuses;

enum
{
    // Channel0__Channel1_E_FUSE
    AUX1__AUX2_E_FUSE = 0,
    ENERGY_METER__COOLING_E_FUSE,
    LEFT_INVERTER__RIGHT_INVERTER_E_FUSE,
    AIR_SHUTDOWN__CAN_POWER_E_FUSE,
    NUM_E_FUSES
};

/**
 * Allocate and initialize an Efuse
 * @param *e_fuse_cs_GPIO_Port Handle to Efuse's chip-select GPIO port
 * @param e_fuse_cs_GPIO_Pin Efuse's chip-select GPIO pin
 * @param *e_fuse_cur_sync_GPIO_Port Handle to Efuse's current-sync GPIO port
 * @param e_fuse_cur_sync_GPIO_Pin Efuse's current-sync GPIO pin
 * @return The created Efuse, whose ownership is given to the caller
 */
struct E_Fuse *App_E_Fuse_Create(
    GPIO_TypeDef *e_fuse_cs_GPIO_Port,
    uint16_t      e_fuse_cs_GPIO_Pin,
    GPIO_TypeDef *e_fuse_cur_sync_GPIO_Port,
    uint16_t      e_fuse_cur_sync_GPIO_Pin);

/**
 * Dallocate the memory used by the given Efuse
 * @param e_fuse The Efuse to deallocate
 */
void App_E_Fuse_Destroy(struct E_Fuse *const e_fuse);

/**
 * Allocate and initialize a group of four Efuses
 * @param e_fuse_1 The first Efuse
 * @param e_fuse_2 The second Efuse
 * @param e_fuse_3 The third Efuse
 * @param e_fuse_4 The fourth Efuse
 * @note This function does __not__ take ownership of any of the Efuses
 *       passed into it, which means the every interface must be kept
 *       alive for the lifetime of this created group of Efuses
 * @return A pointer to the created group of Efuses, whose
 *         ownership is given to the caller
 */
struct E_Fuses *App_E_Fuses_Create(
    struct E_Fuse *const e_fuse_1,
    struct E_Fuse *const e_fuse_2,
    struct E_Fuse *const e_fuse_3,
    struct E_Fuse *const e_fuse_4);

/**
 * Deallocate the memory used by the given group of Efuses
 * @param e_fuses The group of Efuses to deallocate
 */
void App_E_Fuses_Destroy(struct E_Fuses *const e_fuses);

/**
 * Exits the provided Efuse from it's Fail-safe mode and disables the
 * watchdog, to prevent re-entering Fail-safe mode. Disables the
 * Efuse's watchdog timer by setting the Efuse's Global Config Register
 * to the value defined in GCR_CONFIG; where the watchdog is disabled.
 * @param e_fuse Pointer to the Efuse
 */
HAL_StatusTypeDef App_Efuse_22XS4200_ExitFailSafe(struct E_Fuse *e_fuse);

/**
 * Configures the group of Efuses register's using the values defined in:
 * REGNAME_CONFIG
 * @param e_fuses The group of Efuses to configure
 */
void App_Efuse_22XS4200_configureEfuses(struct E_Fuses *e_fuses);

/**
 * Updates the running status of each Efuse
 * @param e_fuses The group of Efuses to update their running status's
 */
void App_Efuse_22XS4200_UpdateEfusesStatus(struct E_Fuses *e_fuses);

/**
 * Updates the fault status of each Efuse
 * @param e_fuses The group of Efuses to update their fault status's
 */
void App_Efuse_22XS4200_UpdateEfusesFaults(struct E_Fuses *e_fuses);
