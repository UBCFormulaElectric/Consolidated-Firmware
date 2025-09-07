#include "app_utils.h"

#include "hw_cordic_h7.h"
#include "hw_utils.h"
#include "main.h"

/**
 * Driver structure
 *
 * To perform a specific operation the peripheral must be configured,
 * I will do some dynamic configuration switching allowing to use any function
 *
 */

static void
    cordic_dispatcher(Cordic *cordic_handle, CORDIC_Function func, float *in_buf, float *out_buf, uint32_t buf_size)
{
    CORDIC_ConfigTypeDef cordic_config = {
        .Function = 0, .Scale = 0, .InSize = buf_size, .OutSize = buf_size, .NbWrite = 0, .NbRead = 0, .Precision = 1
    };

    ExitCode exit = hw_utils_convertHalStatus(HAL_CORDIC_CONFIGURE(cordic_handle, &cordic_handle));

    RETURN_IF_ERR(exit);

    exit =
        hw_utils_convertHalStatus(HAL_CORDIC_CALCULATE(cordic_handle, (int32_t *)in_buf, (int32_t *)out_buf, 0, 1000));

    LOG_IF_ERR(exit);
}

void hw_cordic_init(Cordic *handle) {}

ExitCode c_cos(const float *in_buf, float *out_buf, uint32_t buf_size)
{
    cordic_dispatcher()
}
