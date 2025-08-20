#include "hw_pwms.h"

#define PWM_TIMEOUT_MS (10U)
#define DAC_TIMEOUT_MS (10U)

//PWM Frequency and duty cycle input 
const PwmInput pwm_full_input = { .handle = &htim4};
const PwmInput 

TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim12;

static SpiBus   dac_bus = { .handle = &hspi1 };
const SpiDevice dac     = { .bus = &dac_bus, .nss_pin = &dac_chipSelect, .timeout_ms = DAC_TIMEOUT_MS };

static SpiBus   isospi_bus = { .handle = &hspi2 };
const SpiDevice isospi_ls  = { .bus = &isospi_bus, .nss_pin = &isospi_chipSelect_ls, .timeout_ms = ISOSPI_TIMEOUT_MS };
const SpiDevice isospi_hs  = { .bus = &isospi_bus, .nss_pin = &isospi_chipSelect_hs, .timeout_ms = ISOSPI_TIMEOUT_MS };

// Conversion from SPI HAL handles to the respective bus struct.
SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi1)
        return &dac_bus;
    if (handle == &hspi2)
        return &isospi_bus;
    return NULL;
}

//PWM Frequency only input 


//PWM Frequency and duty cycle output 


extern const PwmInput pwm_full_input;
extern const PwmInput pwm_full_input_dac;

extern const PwmInputFreqOnly pwm_freq_only_input;
extern const PwmInputFreqOnly pwm_freq_only_input_dac;

extern const PwmOutput pwm_full_output;
extern const PwmOutput pwm_full_output_dac;
