#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Io_Efuse.h"
#include "../../Drivers/CMSIS/Device/ST/STM32F3xx/Include/stm32f302xc.h"
#include "../../Drivers/STM32F3xx_HAL_Driver/Inc/stm32f3xx_hal_spi.h"

struct Efuse;
struct Efuse_Context;

/**
 * @param io_efuse              Full Efuse_Context structure extraction
 * @param enable_channel_0      A function that calls IO_function to enable channel_0
 * @param disable_channel_0     A function that calls IO_function to disable channel_0
 * @param enable_channel_1      A function that calls IO_function to enable channel_1
 * @param disable_channel_1     A function that calls IO_function to enable channel_0
 * @param in_fault_mode         A function that verifies if load switch is in fault mode
 * @param in_failsafe_mode      A function that verifies if load switch is in failsafe mode
 * @param delatch_fault         A function that calls delatches the pins in case of error
 * @param get_channel_0_current A function that calls IO_function to retrieve channel_0 current
 * @param get_channel_1_current A function that calls IO_function to retrieve channel_1 current
 * @param channel_0_min_current Minimum current channel 0 can drop to.
 * @param channel_0_max_current Maximum current channel 0 can reach to.
 * @param channel_1_min_current Minimum current channel 1 can drop to.
 * @param channel_1_max_current Maximum current channel 1 can reach to.
 * @return Efuse structure
 */
struct Efuse *App_Efuse_Create(
    struct Efuse_Context(*io_efuse),
    void (*enable_channel_0)(const struct Efuse_Context *const),
    void (*disable_channel_0)(const struct Efuse_Context *const),
    void (*enable_channel_1)(const struct Efuse_Context *const),
    void (*disable_channel_1)(const struct Efuse_Context *const),
    bool (*in_fault_mode)(const struct Efuse_Context *const),
    bool (*in_failsafe_mode)(const struct Efuse_Context *const),
    void (*delatch_fault)(const struct Efuse_Context *const),
    float (*get_channel_0_current)(struct Efuse_Context *const),
    float (*get_channel_1_current)(struct Efuse_Context *const),
    float(channel_0_min_current),
    float(channel_0_max_current),
    float(channel_1_min_current),
    float(channel_1_max_current));

void App_Efuse_Destroy(struct Efuse *efuse);

/**
 * Function to enable channel_0
 * @param efuse App_Efuse structure
 */
void App_Efuse_EnableChannel0(struct Efuse *efuse);

/**
 * Function to disable channel_0
 * @param efuse App_Efuse structure
 */
void App_Efuse_DisableChannel0(struct Efuse *efuse);

/**
 * Function to enable channel_1
 * @param efuse App_Efuse structure
 */
void App_Efuse_EnableChannel1(struct Efuse *efuse);

/**
 * Function to disable channel_1
 * @param efuse App_Efuse structure
 */
void App_Efuse_DisableChannel1(struct Efuse *efuse);

/**
 * Functions checking if Efuse is in fault mode
 * @param efuse App_Efuse structure
 * @return True if Efuse is in Fault Mode, otherwise returns False
 */
bool App_Efuse_IsEfuseInFaultMode(struct Efuse *efuse);

/**
 * Functions checking if Efuse is in failsafe mode
 * @param efuse App_Efuse structure
 * @return True if Efuse is in failsafe Mode, otherwise returns False
 */
bool App_Efuse_IsEfuseInFailSafeMode(struct Efuse *efuse);

/**
 * Delatches Efuse if function is called.
 * @param efuse App_Efuse structure
 */
void App_Efuse_DelatchFaults(struct Efuse *efuse);

/**
 * Functions which get the channel 0 current.
 * @param efuse App_Efuse structure
 * @return The current value of channel_0
 */
float App_Efuse_GetChannel0Current(struct Efuse *efuse);

/**
 * Functions which get the channel 1 current.
 * @param efuse App_Efuse structure
 * @return The current value of channel_1
 */
float App_Efuse_GetChannel1Current(struct Efuse *efuse);

/**
 * Local function that would check the currents to see if they are in range or not.
 * @param value Current value
 * @param min_value Minimum value the current should be.
 * @param max_value Maximum value the current should be.
 * @return True if the current value is in range, otherwise false.
 */
bool App_Efuse_InRangeCheck(float value, float min_value, float max_value);

/**
 * Bool that check if current_0 is in range
 * @param efuse Efuse structure
 * @return True if current_1 is in range, otherwise false.
 */
bool App_Efuse_Channel0_CurrentCheck(struct Efuse *efuse);

/**
 * Bool that check if current_1 is in range
 * @param efuse Efuse structure
 * @return True if current_1 value is in range, otherwise false.
 */
bool App_Efuse_Channel1_CurrentCheck(struct Efuse *efuse);
