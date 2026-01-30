#include "app_powerMonitoring.hpp"

#define CH1 1u // 24
#define CH2 2u // 5
#define CH3 3u // 3

void app_powerMonitoring_update(void)
{
    static bool init_done = false;

    if (!init_done)
    {
        init_done = io_powerMonitoring_init();
    }
    else
    {
        float ch1_voltage = 0.0f;
        float ch1_current = 0.0f;
        float ch1_power   = 0.0f;
        float ch2_voltage = 0.0f;
        float ch2_current = 0.0f;
        float ch2_power   = 0.0f;
        float ch3_voltage = 0.0f;
        float ch3_current = 0.0f;
        float ch3_power   = 0.0f;

        io_powerMonitoring_refresh();
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

// Power manager:
//  24V channel
//  5V channel
//  3V3 channel
//  we have a power pump I2C pins and a Battery monitor I2C