#include <stm32h7xx_hal_cordic.h>
#include <stdint.h>
#include "app_utils.h"

typedef struct
{
    CORDIC_HandleTypeDef handle;
} Cordic;

// typedef enum
// {
//     CORDIC_COS = 0U,
//     CORDIC_SIN,
//     CORDIC_PHASE,
//     CORDIC_MOD,
//     CORDIC_ATAN,
//     CORDIC_COSH,
//     CORDIC_SINH,
//     CORDIC_ATANH,
//     CORDIC_LN,
//     CORDIC_SQRT,
// } CordicFunc;

void hw_cordic_init(Cordic *handle);

// CORDIC operations
ExitCode c_cos(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_sin(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_phase(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_mod(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_atan(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_cosh(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_sinh(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_atanh(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_ln(const float *in_buf, float *out_buf, uint32_t buf_size);
ExitCode c_sqrt(const float *in_buf, float *out_buf, uint32_t buf_size);
