#include "app_powerMonitoring.h"
#include "io_powerMonitoring.h"
#include "app_canTx.h"
#include <cmsis_os2.h>
#include "hw_i2c.h"
#include "hw_i2cs.h"

#define CH1 1u
#define CH2 2u

void app_powerMonitoring_update(void)
{
    static bool init_done = false;

    if (false == init_done)
    {
        init_done = io_powerMonitoring_init();
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
    }
}
