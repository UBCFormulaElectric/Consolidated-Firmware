#include "app_powerMonitoring.h"
#include "io_powerMonitoring.h"
#include "app_canTx.h"
#include <cmsis_os2.h>
#include "hw_i2c.h"
#include "hw_i2cs.h"

#define CH1 1u
#define CH2 2u

#define ALERT_CH1_OVERCURRENT (1UL << 23)
#define ALERT_CH2_OVERCURRENT (1UL << 22)
#define ALERT_CH1_UNDERCURRENT (1UL << 19)
#define ALERT_CH2_UNDERCURRENT (1UL << 18)
#define ALERT_CH1_OVERVOLTAGE (1UL << 15)
#define ALERT_CH2_OVERVOLTAGE (1UL << 14)
#define ALERT_CH1_UNDERVOLTAGE (1UL << 11)
#define ALERT_CH2_UNDERVOLTAGE (1UL << 10)
#define ALERT_CH1_OVERPOWER (1UL << 7)
#define ALERT_CH2_OVERPOWER (1UL << 6)

static const uint32_t alert_mask_map[PWRMON_ALERT_COUNT] = {
    [PWRMON_ALERT_CH1_OVERCURRENT] = ALERT_CH1_OVERCURRENT,   [PWRMON_ALERT_CH2_OVERCURRENT] = ALERT_CH2_OVERCURRENT,
    [PWRMON_ALERT_CH1_UNDERCURRENT] = ALERT_CH1_UNDERCURRENT, [PWRMON_ALERT_CH2_UNDERCURRENT] = ALERT_CH2_UNDERCURRENT,
    [PWRMON_ALERT_CH1_OVERVOLTAGE] = ALERT_CH1_OVERVOLTAGE,   [PWRMON_ALERT_CH2_OVERVOLTAGE] = ALERT_CH2_OVERVOLTAGE,
    [PWRMON_ALERT_CH1_UNDERVOLTAGE] = ALERT_CH1_UNDERVOLTAGE, [PWRMON_ALERT_CH2_UNDERVOLTAGE] = ALERT_CH2_UNDERVOLTAGE
};

void app_powerMonitoring_update(void)
{
    static bool init_done = false;

    if (false == init_done)
    {
        init_done = io_powerMonitoring_init();

        if (init_done)
        {
            osDelay(1);
        }
    }
    else
    {
        float    ch1_voltage = 0.0f;
        float    ch1_current = 0.0f;
        float    ch1_power   = 0.0f;
        float    ch2_voltage = 0.0f;
        float    ch2_current = 0.0f;
        float    ch2_power   = 0.0f;
        uint32_t status      = 0u;

        uint8_t cmd = 0x00;
        hw_i2c_transmit(&pwr_mtr, &cmd, 1);
        osDelay(1);

        io_powerMonitoring_read_alerts(&status);
        io_powerMonitoring_read_voltage(CH1, &ch1_voltage);
        io_powerMonitoring_read_voltage(CH2, &ch2_voltage);
        io_powerMonitoring_read_current(CH1, &ch1_current);
        io_powerMonitoring_read_current(CH2, &ch2_current);
        io_powerMonitoring_read_power(CH1, &ch1_power);
        io_powerMonitoring_read_power(CH2, &ch2_power);

        app_canTx_VC_ChannelOneVoltage_set(ch1_voltage);
        app_canTx_VC_ChannelTwoVoltage_set(ch2_voltage);
        app_canTx_VC_ChannelOneCurrent_set(ch1_current);
        app_canTx_VC_ChannelTwoCurrent_set(ch2_current);
        app_canTx_VC_ChannelOnePower_set(ch1_power);
        app_canTx_VC_ChannelTwoPower_set(ch2_power);

        for (PowerMonitoringAlerts alert = 0; alert < PWRMON_ALERT_COUNT; alert++)
        {
            if (alert == PWRMON_ALERT_NO_ALERTS)
            {
                app_canTx_VC_ChipStatus_set((PowerMonitoringAlerts)alert);
            }
            else if (status & alert_mask_map[alert])
            {
                app_canTx_VC_ChipStatus_set((PowerMonitoringAlerts)alert);
                break;
            }
        }
    }
}
