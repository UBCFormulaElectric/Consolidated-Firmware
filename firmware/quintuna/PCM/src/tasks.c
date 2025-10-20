#include "tasks.h"
#include "app_utils.h"
#include "hw_gpio.h"
#include "main.h"
#include <stdbool.h>
#include "vicor.h"
#include "io_log.h"
#include "hw_hardFaultHandler.h"

// Required by SysView
void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=PCM,D=Cortex-M4,O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

#define LV_ON_THRESHOLD_V (20.0f)
#define TURN_ON_DELAY_MS (100)
typedef enum
{
    PCM_STATE_OFF,
    PCM_STATE_VICOR_ONLY,
    PCM_STATE_ON
} PcmState;

// Define this guy for debug mode.
#define PCM_DEBUG

static const Gpio pcm_en_in      = { .port = PCM_EN_GPIO_Port, .pin = PCM_EN_Pin };
static const Gpio lv_buck_en_out = { .port = LV_BUCK_EN_GPIO_Port, .pin = LV_BUCK_EN_Pin };
static const Gpio led_out        = { .port = LED_GPIO_Port, .pin = LED_Pin };

static PcmState state = PCM_STATE_OFF;

#ifdef PCM_DEBUG
static char debug_buf[1024];
#endif

void tasks_init(void)
{
    hw_hardFaultHandler_init();

    SEGGER_SYSVIEW_Conf();
    LOG_INFO("PCM reset!");

#ifdef PCM_DEBUG
    LOG_IF_ERR(vicor_operation(true));
#endif
}

_Noreturn void tasks_tick(void)
{
    for (;;)
    {
#ifdef PCM_DEBUG
        float vin  = 0;
        float iin  = 0;
        float vout = 0;
        float iout = 0;
        float temp = 0;
        float pout = 0;

        LOG_IF_ERR(vicor_readVin(&vin));
        LOG_IF_ERR(vicor_readIin(&iin));
        LOG_IF_ERR(vicor_readVout(&vout));
        LOG_IF_ERR(vicor_readIout(&iout));
        LOG_IF_ERR(vicor_readTemp(&temp));
        LOG_IF_ERR(vicor_readPout(&pout));

        // SysView does't support floats! :(
        sprintf(
            debug_buf, "Vin = %.2fV\nIin = %.2fA\nVout=%.2fV\nIout=%.2fA\nTemp=%.1fdegC\nPout=%.2fW", (double)vin,
            (double)iin, (double)vout, (double)iout, (double)temp, (double)pout);
        LOG_INFO("%s", debug_buf);

        hw_gpio_togglePin(&led_out);
        osDelay(500);
#else
        switch (state)
        {
            case PCM_STATE_OFF:
            {
                if (hw_gpio_readPin(&pcm_en_in) && IS_EXIT_OK(vicor_clearFaults()) && IS_EXIT_OK(vicor_operation(true)))
                {
                    state = PCM_STATE_VICOR_ONLY;
                }
                break;
            }
            case PCM_STATE_VICOR_ONLY:
            {
                if (!hw_gpio_readPin(&pcm_en_in) && IS_EXIT_OK(vicor_operation(false)))
                {
                    state = PCM_STATE_OFF;
                }

                float vout = 0;
                if (IS_EXIT_OK(vicor_readVout(&vout) && vout >= LV_ON_THRESHOLD_V))
                {
                    osDelay(TURN_ON_DELAY_MS);
                    hw_gpio_writePin(&lv_buck_en_out, true);
                    state = PCM_STATE_ON;
                }
                break;
            }
            case PCM_STATE_ON:
            {
                if (!hw_gpio_readPin(&pcm_en_in))
                {
                    hw_gpio_writePin(&lv_buck_en_out, false);

                    if (IS_EXIT_OK(vicor_operation(false)))
                    {
                        state = PCM_STATE_OFF;
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
