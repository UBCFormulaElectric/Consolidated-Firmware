#include "app_powerMonitoring.h"
#include "io_powerMonitoring.h"
#include "app_canTx.h"

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
        float ch_pcm_voltage = 0.0f;
        float ch_pcm_current = 0.0f;
        float ch_pcm_power   = 0.0f;
        float ch_ext_voltage = 0.0f;
        float ch_ext_current = 0.0f;
        float ch_ext_power   = 0.0f;

        io_powerMonitoring_refresh();
        io_powerMonitoring_read_voltage(CH1, &ch_pcm_voltage);
        io_powerMonitoring_read_voltage(CH2, &ch_ext_voltage);
        io_powerMonitoring_read_current(CH1, &ch_pcm_current);
        io_powerMonitoring_read_current(CH2, &ch_ext_current);
        io_powerMonitoring_read_power(CH1, &ch_pcm_power);
        io_powerMonitoring_read_power(CH2, &ch_ext_power);

        app_canTx_VC_PcmChannelVoltage_set(ch_pcm_voltage);
        app_canTx_VC_ExtChannelVoltage_set(ch_ext_voltage);
        app_canTx_VC_PcmChannelCurrent_set(ch_pcm_current);
        app_canTx_VC_ExtChannelCurrent_set(ch_ext_current);
        app_canTx_VC_PcmChannelPower_set(ch_pcm_power);
        app_canTx_VC_ExtChannelPower_set(ch_ext_power);
    }
}
