#include "jobs.hpp"

#include "app_jsoncan.hpp"
#include "io_canQueues.hpp"
#include "io_canTx.hpp"
#include "io_canRx.hpp"
#include "io_log.hpp"
#include "efuse/io_efuse_TI_TPS28.hpp"
#include "hw_gpio.hpp"
#include "hw_adc.hpp"
#include <io_canMsg.hpp>
#include <util_errorCodes.hpp>
#include "io_math.hpp"
#include <cmath>

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
<<<<<<< HEAD
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(can_tx_queue.push(msg));
        });

    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
=======
    // adc1.init();
    // efuse.setChannel(true);
    // LOG_INFO("IS CHANNEL ENABLED: %s", efuse.isChannelEnabled() ? "YES" : "NO");
>>>>>>> 3b22b446c (compiles)
}

void jobs_run1Hz_tick() {}

void jobs_run100Hz_tick()
{
    // float current = efuse.getChannelCurrent() / 1.720f;
    // bool  ok      = efuse.ok();

    // LOG_INFO("CURRENT: %d", (int)(current * 100000));
<<<<<<< HEAD
<<<<<<< HEAD
=======
    auto result = ccos(0.0f);
    LOG_INFO("COS(0.0) = %f", static_cast<double>(result.value_or(0.0f)));
>>>>>>> 3b22b446c (compiles)
=======
    auto  cordic_result = io::math::ccos(0.0f);
    float libc_result   = std::cosf(0.0f);
    float diff          = std::abs(cordic_result.value_or(0.0f) - libc_result);
    LOG_INFO("COS(0.0) = %f, DIFF: %f", static_cast<double>(cordic_result.value_or(0.0f)), static_cast<double>(diff));
>>>>>>> 35194ecf2 (little changes)
}

void jobs_run1kHz_tick() {}
