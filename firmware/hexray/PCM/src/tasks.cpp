#include "tasks.h"
#include "hw_gpio.hpp"
#include "main.h"
#include "vicor.hpp"
#include "io_log.hpp"
#include "hw_hardFaultHandler.hpp"
#include "SEGGER_SYSVIEW.h"
#include "cmsis_os2.h"
#include "hw_rtosTaskHandler.hpp"

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
    LV,
    OFF,
    VICOR_ONLY,
    ON
};

// Define this guy for debug mode.
// #define PCM_DEBUG

static const hw::gpio pcm_en_in{ PCM_EN_GPIO_Port, PCM_EN_Pin };
static const hw::gpio lv_buck_en_out{ LV_BUCK_EN_GPIO_Port, LV_BUCK_EN_Pin };
static const hw::gpio led_out{ LED_GPIO_Port, LED_Pin };

static auto state = PcmState::LV;

#ifdef PCM_DEBUG
static char debug_buf[1024];
#endif

[[noreturn]] static void tasks_tick(void *arg)
{
    forever
    {
#ifdef PCM_DEBUG
        // SysView does't support floats! :(
        // TODO fix
        sprintf(
            debug_buf, "Vin = %.2fV\nIin = %.2fA\nVout=%.2fV\nIout=%.2fA\nTemp=%.1fdegC\nPout=%.2fW",
            static_cast<double>(vin_res.value_or(0)), static_cast<double>(iin_res.value_or(0)),
            static_cast<double>(vout_res.value_or(0)), static_cast<double>(iout_res.value_or(0)),
            static_cast<double>(temp_res.value_or(0)), static_cast<double>(pout_res.value_or(0)));
        LOG_INFO("%s", debug_buf);

        UNUSED(led_out.togglePin());
        osDelay(500);
#else
        const result<vicor::status::General> status_res   = vicor::status::general();
        bool                                 should_clear = false;
        if (status_res.has_value())
        {
            // status_res.value().log();
            if (status_res.value().status_mfr_specific)
            {
                if (const result<vicor::status::MFRSpecific> specific_status_res = vicor::status::mfrspecific();
                    specific_status_res.has_value())
                {
                    // specific_status_res.value().log();
                    should_clear |= specific_status_res.value().any();
                }
                else
                {
                    continue;
                }
            }
            should_clear |= status_res.value().any();
        }
        else
        {
            continue;
        }

        const result<vicor::status::Comm> status_comm_res = vicor::status::comm();
        LOG_IF_ERR(status_comm_res);
        if (status_comm_res.has_value())
        {
            // status_comm_res.value().log();
            LOG_IF_ERR(vicor::clearFaults()); // TODO figure out where to put this
        }
        else
        {
            continue;
        }

        const result<vicor::status::CurrentOutput> current = vicor::status::iout();
        LOG_IF_ERR(current);
        if (current.has_value())
        {
            // current.value().log();
        }
        else
        {
            continue;
        }

        const result<vicor::status::Temp> temp = vicor::status::temp();
        LOG_IF_ERR(temp);
        if (temp.has_value())
        {
            // temp.value().log();
        }
        else
        {
            continue;
        }

        if (should_clear)
        {
            // LOG_INFO("Clearing faults because status indicates there are faults");
            LOG_IF_ERR(vicor::clearFaults());
        }

        // all above variables should have values
        const vicor::status::General       &general_status = status_res.value();
        const vicor::status::Comm          &comm_status    = status_comm_res.value();
        const vicor::status::CurrentOutput &current_status = current.value();
        const vicor::status::Temp          &temp_status    = temp.value();

        switch (state)
        {
            case PcmState::LV: // this state is for when the PCM is awake, but the vicor is still off
            {
                if (not general_status.unit_off) // namely unit_on
                {
                    LOG_INFO("Going to OFF state");
                    state = PcmState::OFF;
                }
                break;
            }
            case PcmState::OFF: // this state is for when HV is off, or when the HV is on, but the vicor is still off
                                // (precharge)
            {
                if (pcm_en_in.readPin())
                {
                    LOG_INFO("trying to clear faults and turn on");
                    if (vicor::clearFaults().has_value() and vicor::operation(true).has_value())
                    {
                        LOG_INFO("Going to VICOR_ONLY state");
                        state = PcmState::VICOR_ONLY;
                    }
                }
                else
                {
                    LOG_IF_ERR(vicor::operation(false)); // PLEASE!!!!!!! TURN OFF!!!!!!
                }
                break;
            }
            case PcmState::VICOR_ONLY:
            {
                if (not pcm_en_in.readPin() and vicor::operation(false).has_value())
                {
                    LOG_INFO("Going to OFF state");
                    state = PcmState::OFF;
                }

                const result<vicor::read::Power> power_info = vicor::read::power_stats();
                LOG_IF_ERR(power_info);

                if (power_info.has_value() and power_info->vout >= LV_ON_THRESHOLD_V)
                {
                    osDelay(TURN_ON_DELAY_MS);
                    lv_buck_en_out.writePin(true);
                    LOG_INFO("Going to ON state");
                    state = PcmState::ON;
                }
                break;
            }
            case PcmState::ON: // everything on
            {
                if (not pcm_en_in.readPin())
                {
                    lv_buck_en_out.writePin(false);

                    if (vicor::operation(false).has_value())
                    {
                        LOG_INFO("Going to OFF state");
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

        if (general_status.unit_off and state != PcmState::LV)
        {
            LOG_INFO("Going to LV state because unit is off");
            state = PcmState::LV;
        }
#endif
    }
}

static hw::rtos::StaticTask::StaticTaskStack<8096> TaskPcmStack;
static hw::rtos::StaticTask                        TaskPcm(osPriorityHigh, "TaskPCM", tasks_tick, TaskPcmStack);

void tasks_init()
{
    // hw_hardFaultHandler_init();

    SEGGER_SYSVIEW_Conf();
    LOG_INFO("PCM reset!");

#ifdef PCM_DEBUG
    LOG_IF_ERR(vicor_operation(true));
#else
    if (const auto metadata_res = vicor::read::metadata(); metadata_res.has_value())
    {
        metadata_res.value().log();
    }
    else
    {
        LOG_INFO("Failed to read metadata");
    }

    if (const auto limits_res = vicor::read::limits(); limits_res.has_value())
    {
        limits_res.value().log();
    }
    else
    {
        LOG_INFO("Failed to read limits");
    }

    if (const auto capability_res = vicor::read::capability(); capability_res.has_value())
    {
        capability_res.value().log();
    }
    else
    {
        LOG_INFO("Failed to read capability");
    }
#endif

    osKernelInitialize();
    TaskPcm.start();
    osKernelStart();
    forever {}
}