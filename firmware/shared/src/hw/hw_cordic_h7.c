#include <math.h>

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

CORDIC_ConfigTypeDef cordic_config = { .Function  = func,
                                       .Scale     = CORDIC_SCALE_0,
                                       .InSize    = CORDIC_INSIZE_16BITS,
                                       .OutSize   = CORDIC_OUTSIZE_16BITS,
                                       .NbWrite   = 0,
                                       .NbRead    = 0,
                                       .Precision = CORDIC_PRECISION_6CYCLES };

static ExitCode cordic_dispatcher(
    CORDIC_Function func,
    CORDIC_Nb_Write num_res,
    CORDIC_Nb_Write num_args,
    CORDIC_Scale    scale,
    int16_t          *in_buf,
    int16_t        *out_buf,
    uint32_t        buf_size)
{
    cordic_config.Function = func;
    cordic_config.NbWrite   = num_res;
    cordic_config.NbRead    = num_args;

    ExitCode exit = hw_utils_convertHalStatus(HAL_CORDIC_Configure(&hcordic, &cordic_config));

    RETURN_IF_ERR(exit);

    exit = hw_utils_convertHalStatus(
        HAL_CORDIC_CalculateZO(cordic_handle, (int16_t *)in_buf, (int16_t *)out_buf, buf_size, 1000));

    return exit;
}

float c_cos(const float angle_rad, const float modulus)
{
    if (modulus == 0.0f)
        return 0.0f;

    int16_t result;
    int16_t in_buf = { FLOAT_TO_Q1_15(angle_rad / M_PI), FLOAT_TO_Q1_15(modulus) };

    // add scaling if modulus > 1 or adapt to Q1.15

    ExitCode exit = cordic_dispatcher(
        CORDIC_FUNCTION_COSINE, CORDIC_NBREAD_2, CORDIC_NBWRITE_1, CORDIC_SCALE_0, &in_buf, &result, 1);
    LOG_IF_ERR(exit);

    return Q1_15_TO_FLOAT(result);
}

float c_sin(const float angle_rad, const float modulus)
{
    if (modulus == 0.0f)
        return 0.0f;

    int16_t result;
    int16_t in_buf = { FLOAT_TO_Q1_15(angle_rad / M_PI), FLOAT_TO_Q1_15(modulus) };

    // add scaling if modulus > 1 or adapt to Q1.15

    ExitCode exit =
        cordic_dispatcher(CORDIC_FUNCTION_SINE, CORDIC_NBREAD_2, CORDIC_NBWRITE_1, CORDIC_SCALE_0, &in_buf, &result, 1);
    LOG_IF_ERR(exit);

    return Q1_15_TO_FLOAT(result);
}

float c_phase(const float x, const float y)
{

}
