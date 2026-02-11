#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
extern "C"
{
#include "app_canTx.h"
}

namespace app::powerMonitoring
{
constexpr uint8_t CH1 = 1u; // 24V
constexpr uint8_t CH2 = 2u; // 5V
constexpr uint8_t CH3 = 3u; // 3V

void update()
{
    static bool init_done = false;

    for (int tries = 0; not init_done and tries < 10; tries++)
    {
        init_done = io_powerMonitoring_init();
    }
    if(not init_done) {
        // error handle
        return;
    }
    
    // float ch3_voltage = 0.0f;
    // float ch3_current = 0.0f;
    // float ch3_power   = 0.0f;

    io::powerMonitoring::refresh();
    const float ch1_voltage = io::powerMonitoring::read_voltage(CH1);
    const float ch2_voltage = io::powerMonitoring::read_voltage(CH2);
    const float ch1_current = io::powerMonitoring::read_current(CH1);
    const float ch2_current = io::powerMonitoring::read_current(CH2);
    const float ch1_power   = io::powerMonitoring::read_power(CH1);
    const float ch2_power   = io::powerMonitoring::read_power(CH2);

    app_canTx_VC_ChannelOneVoltage_set(ch1_voltage);
    app_canTx_VC_ChannelTwoVoltage_set(ch2_voltage);
    app_canTx_VC_ChannelOneCurrent_set(ch1_current);
    app_canTx_VC_ChannelTwoCurrent_set(ch2_current);
    app_canTx_VC_ChannelOnePower_set(ch1_power);
    app_canTx_VC_ChannelTwoPower_set(ch2_power);
}

} // namespace app::powerMonitoring
  // Power manager:
  //  24V channel
  //  5V channel
  //  3V3 channel
  //  we have a power pump I2C pins and a Battery monitor I2C