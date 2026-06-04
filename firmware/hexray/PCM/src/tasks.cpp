#include "tasks.h"
#include "hw_gpio.hpp"
#include "main.h"
#include "vicor.hpp"
#include "io_log.hpp"
#include "hw_hardFaultHandler.hpp"
#include "SEGGER_SYSVIEW.h"
#include "cmsis_os2.h"
#include <cstdio>

// Required by SysView
void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=PCM,D=Cortex-M4,O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

static constexpr float    LV_ON_THRESHOLD_V = 20.0f;
static constexpr uint32_t TURN_ON_DELAY_MS  = 100;
enum class PcmState
{
    OFF,
    VICOR_ONLY,
    ON
};

// Define this guy for debug mode.
// #define PCM_DEBUG

static const hw::gpio pcm_en_in{ PCM_EN_GPIO_Port, PCM_EN_Pin };
static const hw::gpio lv_buck_en_out{ LV_BUCK_EN_GPIO_Port, LV_BUCK_EN_Pin };
static const hw::gpio led_out{ LED_GPIO_Port, LED_Pin };

static auto state = PcmState::OFF;

#ifdef PCM_DEBUG
static char debug_buf[1024];
#endif

void tasks_init()
{
    hw_hardFaultHandler_init();

    SEGGER_SYSVIEW_Conf();
    LOG_INFO("PCM reset!");

#ifdef PCM_DEBUG
    LOG_IF_ERR(vicor_operation(true));
#endif
}

_Noreturn void tasks_tick()
{
    for (;;)
    {
#ifdef PCM_DEBUG
        const auto vin_res  = vicor_readVin();
        const auto iin_res  = vicor_readIin();
        const auto vout_res = vicor_readVout();
        const auto iout_res = vicor_readIout();
        const auto temp_res = vicor_readTemp();
        const auto pout_res = vicor_readPout();

        LOG_IF_ERR(vin_res);
        LOG_IF_ERR(iin_res);
        LOG_IF_ERR(vout_res);
        LOG_IF_ERR(iout_res);
        LOG_IF_ERR(temp_res);
        LOG_IF_ERR(pout_res);

        // SysView does't support floats! :(
        sprintf(
            debug_buf, "Vin = %.2fV\nIin = %.2fA\nVout=%.2fV\nIout=%.2fA\nTemp=%.1fdegC\nPout=%.2fW",
            static_cast<double>(vin_res.value_or(0)), static_cast<double>(iin_res.value_or(0)),
            static_cast<double>(vout_res.value_or(0)), static_cast<double>(iout_res.value_or(0)),
            static_cast<double>(temp_res.value_or(0)), static_cast<double>(pout_res.value_or(0)));
        LOG_INFO("%s", debug_buf);

        UNUSED(led_out.togglePin());
        osDelay(500);
#else
        switch (state)
        {
            case PcmState::OFF:
            {
                if (pcm_en_in.readPin() and vicor_clearFaults().has_value() and vicor_operation(true).has_value())
                {
                    state = PcmState::VICOR_ONLY;
                }
                break;
            }
            case PcmState::VICOR_ONLY:
            {
                if (not pcm_en_in.readPin() and vicor_operation(false).has_value())
                {
                    state = PcmState::OFF;
                }

                // float vout = ;
                if (const auto vout = vicor_readVout(); vout.has_value() && vout.value() >= LV_ON_THRESHOLD_V)
                {
                    osDelay(TURN_ON_DELAY_MS);
                    lv_buck_en_out.writePin(true);
                    state = PcmState::ON;
                }
                break;
            }
            case PcmState::ON:
            {
                if (not pcm_en_in.readPin())
                {
                    lv_buck_en_out.writePin(false);

                    if (vicor_operation(false).has_value())
                    {
                        state = PcmState::OFF;
                    }
                }
                break;
            }
            default:
            {
                // Should never get here.
                break;
            }
        }
#endif
    }
}
