#include "jobs.hpp"

#include "io_log.hpp"
#include "efuse/io_efuse_TI_TPS28.hpp"
#include "hw_gpio.hpp"
#include "hw_adc.hpp"

extern "C"
{
#include "main.h"
}

using namespace hw;
using namespace io;

const Gpio                      efuse_en{ EFUSE_EN_GPIO_Port, EFUSE_EN_Pin };
const Gpio                      diag_en{ DIAG_EN_GPIO_Port, DIAG_EN_Pin };
const size_t                    NUM_ADC_CHANNELS = 1U;
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1, &htim3 };
const Adc                       efuse_i_sns( adc1.getChannel(0) );
TI_TPS28_Efuse                  efuse( efuse_en, efuse_i_sns, diag_en );

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc1.update_callback();
}

void jobs_init()
{
    // adc1.init();
    efuse.setChannel(true);
    efuse.enableDiagnostics(true);
    // LOG_INFO("IS CHANNEL ENABLED: %s", efuse.isChannelEnabled() ? "YES" : "NO");
}

void jobs_run1Hz_tick() {}

void jobs_run100Hz_tick()
{
    float current = efuse.getChannelCurrent();
    bool  ok      = efuse.ok();

    // LOG_INFO("CURRENT: %.3f, EFUSE OK: %s", static_cast<double>(current), ok ? "YES" : "NO");
}

void jobs_run1kHz_tick() {}
