#include <stm32f3xx.h>
#include "Io_SharedAdc.h"
#include "Io_Adc.h"

enum
{
    CHANNEL_12,
    NUM_ADC_CHANNLES
};

static uint16_t raw_adc_values[NUM_ADC_CHANNLES];
static float    adc_voltages[NUM_ADC_CHANNLES];

#include <cmsis_os.h>
#include <App_CanTx.h>
extern struct DimCanTxInterface *can_tx;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    const struct CanMsgs_dim_watchdog_timeout_t payload;
    App_CanTx_SendNonPeriodicMsg_DIM_WATCHDOG_TIMEOUT(can_tx, &payload);

    adc_voltages[CHANNEL_12] =
        Convert_ADC_Voltage(hadc, raw_adc_values[CHANNEL_12]);
}

uint16_t *Io_Adc_GetRawAdcReadings(void)
{
    return raw_adc_values;
}
