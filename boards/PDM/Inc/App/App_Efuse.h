#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Io_Efuse.h"

struct Efuse;
struct Io_Efuse;

/**
 * @param io_efuse              Full Io_Efuse structure extraction
 * @param enable_channel_0      A function that calls IO_function to enable channel_0
 * @param disable_channel_0     A function that calls IO_function to disable channel_0
 * @param enable_channel_1      A function that calls IO_function to enable channel_1
 * @param disable_channel_1     A function that calls IO_function to enable channel_0
 * @param is_channel_0_enabled  Bool checking if channel 0 is enabled
 * @param is_channel_1_enabled  Bool checking if channel 1 is enabled
 * @param get_channel_0_current A function that calls IO_function to retrieve channel_0 current
 * @param get_channel_1_current A function that calls IO_function to retrieve channel_1 current
 * @param channel_0_min_current Minimum current channel 0 can drop to.
 * @param channel_0_max_current Maximum current channel 0 can reach.
 * @param channel_1_min_current Minimum current channel 1 can drop to.
 * @param channel_1_max_current Maximum current channel 1 can reach.
 * @return Efuse structure
 */
struct Efuse *App_Efuse_Create(
    struct Io_Efuse(*io_efuse),
    void (*enable_channel_0)(struct Io_Efuse *),
    void (*disable_channel_0)(struct Io_Efuse *),
    void (*enable_channel_1)(struct Io_Efuse *),
    void (*disable_channel_1)(struct Io_Efuse *),
    bool (*is_channel_0_enabled)(struct Io_Efuse *),
    bool (*is_channel_1_enabled)(struct Io_Efuse *),
    void (*stdby_reset)(struct Io_Efuse *),
    float (*get_channel_0_current)(struct Io_Efuse *),
    float (*get_channel_1_current)(struct Io_Efuse *),
    float channel_0_min_current,
    float channel_0_max_current,
    float channel_1_min_current,
    float channel_1_max_current);

void App_Efuse_Destroy(struct Efuse *efuse);

/**
 * Function to enable channel_0 of the PDM Efuse
 * @param efuse App_Efuse structure
 */
void App_Efuse_EnableChannel0(struct Efuse *efuse);

/**
 * Function to disable channel_0 of the PDM Efuse
 * @param efuse App_Efuse structure
 */
void App_Efuse_DisableChannel0(struct Efuse *efuse);

/**
 * Function to enable channel_1 of the PDM Efuse
 * @param efuse App_Efuse structure
 */
void App_Efuse_EnableChannel1(struct Efuse *efuse);

/**
 * Function to disable channel_1 of the PDM Efuse
 * @param efuse App_Efuse structure
 */
void App_Efuse_DisableChannel1(struct Efuse *efuse);

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
 * @return 0 if fault, 1 if fine
 */
int App_Efuse_FaultProcedureChannel0(struct Efuse *efuse, int max_attempts);

/**
 * If current too low & enabled or current > max current, then it will go through this fault procedure function
 * (channel1)
 * @param efuse
 * @param max_attempts max number of attempts allowed before faulting
 * @return 0 if fault, 1 if fine
 */
int App_Efuse_FaultProcedureChannel1(struct Efuse *efuse, int max_attempts);
