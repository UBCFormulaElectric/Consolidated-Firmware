#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
extern "C"
{
#include "app_canTx.h"
}
  // Power manager:
  //  we have a power pump I2C pins and a Battery monitor I2C

namespace app::powerMonitoring
{
constexpr uint8_t CH1 = 1u; // V
constexpr uint8_t CH2 = 2u; // V
constexpr uint8_t CH3 = 3u; // V
constexpr uint8_t Ch4 = 4u; // V


void update()
{
    static bool init_done = false;

    for (int tries = 0; not init_done and tries < 10; tries++)
    {
        init_done = io::powerMonitoring::init();
    }
    if(not init_done) {
        // error handle
        std::unexpected(ErrorCode::ERROR);
    }
    
    io::powerMonitoring::refresh();
    const float ch1_voltage = io::powerMonitoring::read_voltage(CH1);
    const float ch2_voltage = io::powerMonitoring::read_voltage(CH2);
    const float ch3_voltage = io::powerMonitoring::read_voltage(CH3);
    const float ch4_voltage = io::powerMonitoring::read_voltage(CH4);

    const float ch1_current = io::powerMonitoring::read_current(CH1);
    const float ch2_current = io::powerMonitoring::read_current(CH2);
    const float ch3_current = io::powerMonitoring::read_current(CH3);
    const float ch4_current = io::powerMonitoring::read_current(CH4);

    const float ch1_power   = io::powerMonitoring::read_power(CH1);
    const float ch2_power   = io::powerMonitoring::read_power(CH2);
    const float ch3_power   = io::powerMonitoring::read_power(CH3);
    const float ch4_power   = io::powerMonitoring::read_power(CH4);

    app_canTx_VC_ChannelOneVoltage_set(ch1_voltage);
    app_canTx_VC_ChannelTwoVoltage_set(ch2_voltage);
    app_canTx_VC_ChannelThreeVoltage_set(ch3_voltage);
    app_canTx_VC_ChannelFourVoltage_set(ch3_voltage);

    app_canTx_VC_ChannelOneCurrent_set(ch1_current);
    app_canTx_VC_ChannelTwoCurrent_set(ch2_current);
    app_canTx_VC_ChannelThreeCurrent_set(ch3_current);
    app_canTx_VC_ChannelFourCurrent_set(ch3_current);

    app_canTx_VC_ChannelOnePower_set(ch1_power);
    app_canTx_VC_ChannelTwoPower_set(ch2_power);
    app_canTx_VC_ChannelThreePower_set(ch3_power);
    app_canTx_VC_ChannelFourPower_set(ch3_power);
}

} // namespace app::powerMonitoring