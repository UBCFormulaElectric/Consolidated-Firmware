#include "jobs.hpp"

#include "io_log.hpp"
#include "efuse/io_efuse_TI_TPS28.hpp"
#include "hw_gpio.hpp"
#include "hw_adc.hpp"
#include "util_math.hpp"

extern "C"
{
#include "main.h"
}

using namespace hw;
using namespace io;

const Gpio                      efuse_en{ EFUSE_EN_GPIO_Port, EFUSE_EN_Pin };
const Gpio                      efuse_pgood{ EFUSE_PGOOD_GPIO_Port, EFUSE_PGOOD_Pin };
const size_t                    NUM_ADC_CHANNELS = 1U;
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1, &htim3 };
const Adc                       efuse_i_sns(adc1.getChannel(0));
TI_TPS28_Efuse                  efuse(efuse_en, efuse_i_sns, efuse_pgood);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc1.update_callback();
}

void jobs_init()
{
    // adc1.init();
    // efuse.setChannel(true);
    // LOG_INFO("IS CHANNEL ENABLED: %s", efuse.isChannelEnabled() ? "YES" : "NO");
}

void jobs_run1Hz_tick() {}

void jobs_run100Hz_tick()
{
    // float current = efuse.getChannelCurrent() / 1.720f;
    // bool  ok      = efuse.ok();

    // LOG_INFO("CURRENT: %d", (int)(current * 100000));
    auto result = ccos(0.0f);
    LOG_INFO("COS(0.0) = %f", static_cast<double>(result.value_or(0.0f)));
}

void jobs_run1kHz_tick() {}
