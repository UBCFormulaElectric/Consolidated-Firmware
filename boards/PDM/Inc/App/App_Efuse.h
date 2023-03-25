#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Io_Efuse.h"

struct Efuse;

/**
 *
 * @param io_efuse_channel0 Io_Efuse Enum value for channel 0
 * @param io_efuse_channel1 Io_Efuse Enum value for channel 1
 * @param channel_0_min_current Minimum acceptable current for channel 0
 * @param channel_0_max_current Maximum acceptable current for channel 0
 * @param channel_1_min_current Minimum acceptable current for channel 1
 * @param channel_1_max_current Maximum acceptable current for channel 1
 * @param max_reset_attempts Maximum attempts allowed for fault checks for this efuse
 * @return
 */
struct Efuse *App_Efuse_Create(
        EfuseChannel io_efuse_channel0,
        EfuseChannel io_efuse_channel1,
        float channel_0_min_current,
        float channel_0_max_current,
        float channel_1_min_current,
        float channel_1_max_current,
        int   max_reset_attempts);

void App_Efuse_Destroy(struct Efuse *efuse);

/**
 * Function to enable or disable channel 0 for the PDM Efuse
 * @param efuse
 * @param status true to enable, false to disable
 */
void App_Efuse_EnableChannel0(struct Efuse *efuse, bool status);

/**
 * Function to enable or disable channel 1 for the PDM Efuse
 * @param efuse
 * @param status true to enable, false to disable
 */
void App_Efuse_EnableChannel1(struct Efuse *efuse, bool status);

/**
 * Function that checks if channel_0 is enabled of the PDM Efuse
 * @param efuse
 * @return true if channel_0 is enabled, false otherwise
 */
bool App_Efuse_IsChannel0Enabled(struct Efuse *efuse);

/**
 * Function that checks if channel_1 is enabled of the PDM Efuse
 * @param efuse
 * @return true if channel_1 is enabled, false otherwise
 */
bool App_Efuse_IsChannel1Enabled(struct Efuse *efuse);

/**
 * Function to attempt to reset the Efuse
 * @param efuse
 */
void App_Efuse_StandbyReset(struct Efuse *efuse);

/**
 * Gets the current value of channel_0 of the PDM Efuse in Amps
 * @param efuse
 * @return The current value of channel_0
 */
float App_Efuse_GetChannel0Current(struct Efuse *efuse);

/**
 * Gets the current value of channel_1 of the PDM Efuse in Amps
 * @param efuse App_Efuse structure
 * @return The current value of channel_1
 */
float App_Efuse_GetChannel1Current(struct Efuse *efuse);

/**
 * Checks if the current of the channel_0 Efuse is too low
 * @param efuse
 * @return true if current too low, false if good (channel0)
 */
bool App_Efuse_Channel0_CurrentLowCheck(struct Efuse *efuse);

/**
 * Checks if the current of the channel_0 Efuse is too high
 * @param efuse
 * @return true if current too high, false if good (channel0)
 */
bool App_Efuse_Channel0_CurrentHighCheck(struct Efuse *efuse);

/**
 * Checks if the current of the channel_1 Efuse is too low
 * @param efuse
 * @return true if current too high, false if good (channel0)
 */
bool App_Efuse_Channel1_CurrentLowCheck(struct Efuse *efuse);

/**
 * Checks if the current of the channel_1 Efuse is too high
 * @param efuse
 * @return true if current too high, false if good (channel1)
 */
bool App_Efuse_Channel1_CurrentHighCheck(struct Efuse *efuse);

/**
 * If current too low & enabled or current > max current, then it will go through this fault procedure function
 * (channel0)
 * @param efuse
 * @param max_attempts max number of attempts allowed before faulting
 * @return true if fault, false if fine
 */
bool App_Efuse_FaultCheckChannel0(struct Efuse *efuse);

/**
 * If current too low & enabled or current > max current, then it will go through this fault procedure function
 * (channel1)
 * @param efuse
 * @param max_attempts max number of attempts allowed before faulting
 * @return true if fault, false if fine
 */
bool App_Efuse_FaultCheckChannel1(struct Efuse *efuse);
