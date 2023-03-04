#pragma once

#include <assert.h>
#include <stdbool.h>
#include "Io_Efuse.h"

struct Io_Efuse;
/**
 *
 * @return io_efuse structure
 */
struct Io_Efuse *Io_Efuse_Create(void);

void Io_Efuse_Destroy(struct Io_Efuse *io_efuse);

/**
 * Enables Channel_0 for the Efuse
 * @param io_efuse
 */
void Io_Efuse_EnableChannel0(struct Io_Efuse *io_efuse);

/**
 * Disables Channel_0 for the Efuse
 * @param io_efuse
 */
void Io_Efuse_DisableChannel0(struct Io_Efuse *io_efuse);

/**
 * Enables Channel_1 for the Efuse
 * @param io_efuse
 */
void Io_Efuse_EnableChannel1(struct Io_Efuse *io_efuse);

/**
 * Disables Channel_1 for the Efuse
 * @param io_efuse
 */
void Io_Efuse_DisableChannel1(struct Io_Efuse *io_efuse);

/**
 * Checks if Channel_0 is enabled or not on the Efuse
 * @param io_efuse
 * return true if enabled, false if not
 */
bool Io_Efuse_IsChannel0Enabled(struct Io_Efuse *io_efuse);

/**
 * Checks if Channel_1 is enabled or not on the Efuse
 * @param io_efuse
 * @return true if enabled, false if not
 */
bool Io_Efuse_IsChannel1Enabled(struct Io_Efuse *io_efuse);

/**
 * Puts a current through the STBY_RST pin to attempt to reset the Efuse after a current failure
 * @param io_efuse
 */
void Io_Efuse_StandbyReset(struct Io_Efuse *io_efuse);

/**
 * Gets the channel_0 current of the Efuse
 * @param io_efuse
 * @return Channel_0 Current Value
 */
float Io_Efuse_GetChannel0Current(struct Io_Efuse *io_efuse);

/**
 * Returns the channel_1 current of the Efuse
 * @param io_efuse
 * @return Channel_1 Current Value
 */
float Io_Efuse_GetChannel1Current(struct Io_Efuse *io_efuse);
