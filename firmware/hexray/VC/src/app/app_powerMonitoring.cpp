#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
#include "io_powerMonitoring_datatypes.hpp"
#include "app_canTx.hpp"
#include "hw_gpios.hpp"

// Power manager:
//  we have a power pump I2C pins and a Battery monitor I2C

namespace app::powerMonitoring
{
constexpr uint8_t CH1 = 1u; // V
constexpr uint8_t CH2 = 2u; // V
constexpr uint8_t CH3 = 3u; // V
constexpr uint8_t CH4 = 4u; // V

void update()
{
    static bool init_done = false;

    for (int tries = 0; not init_done and tries < 10; tries++)
    {
        init_done = io::powerMonitoring::init().has_value();
    }
    if (not init_done)
    {
        // error handle
        return;
    }

    if (!io::powerMonitoring::refresh().has_value())
    {
        return;
    }

    if (!pwr_mtr_nalert.readPin())
    {
        const auto alert_status_res = io::powerMonitoring::read_alert_status();
        if (alert_status_res.has_value())
        {
            const uint8_t OV_UV_mask = alert_status_res.value();

            app::can_tx::VC_AlertOVChannel1_set((OV_UV_mask & ALERT_OV_CH1) != 0u);
            app::can_tx::VC_AlertOVChannel2_set((OV_UV_mask & ALERT_OV_CH2) != 0u);
            app::can_tx::VC_AlertOVChannel3_set((OV_UV_mask & ALERT_OV_CH3) != 0u);
            app::can_tx::VC_AlertOVChannel4_set((OV_UV_mask & ALERT_OV_CH4) != 0u);
            app::can_tx::VC_AlertUVChannel1_set((OV_UV_mask & ALERT_UV_CH1) != 0u);
            app::can_tx::VC_AlertUVChannel2_set((OV_UV_mask & ALERT_UV_CH2) != 0u);
            app::can_tx::VC_AlertUVChannel3_set((OV_UV_mask & ALERT_UV_CH3) != 0u);
            app::can_tx::VC_AlertUVChannel4_set((OV_UV_mask & ALERT_UV_CH4) != 0u);
        }
    }

    const auto ch1_voltage_res = io::powerMonitoring::read_voltage(CH1);
    const auto ch2_voltage_res = io::powerMonitoring::read_voltage(CH2);
    const auto ch3_voltage_res = io::powerMonitoring::read_voltage(CH3);
    const auto ch4_voltage_res = io::powerMonitoring::read_voltage(CH4);
    const auto ch1_current_res = io::powerMonitoring::read_current(CH1);
    const auto ch2_current_res = io::powerMonitoring::read_current(CH2);
    const auto ch3_current_res = io::powerMonitoring::read_current(CH3);
    const auto ch4_current_res = io::powerMonitoring::read_current(CH4);
    const auto ch1_power_res = io::powerMonitoring::read_power(CH1);
    const auto ch2_power_res = io::powerMonitoring::read_power(CH2);
    const auto ch3_power_res = io::powerMonitoring::read_power(CH3);
    const auto ch4_power_res = io::powerMonitoring::read_power(CH4);

    if (!ch1_voltage_res.has_value() || !ch2_voltage_res.has_value() || !ch3_voltage_res.has_value() || !ch4_voltage_res.has_value() ||
        !ch1_current_res.has_value() || !ch2_current_res.has_value() || !ch3_current_res.has_value() || !ch4_current_res.has_value() ||
        !ch1_power_res.has_value() || !ch2_power_res.has_value() || !ch3_power_res.has_value() || !ch4_power_res.has_value())
    {
        return;
    }

    const float ch1_voltage = ch1_voltage_res.value();
    const float ch2_voltage = ch2_voltage_res.value();
    const float ch3_voltage = ch3_voltage_res.value();
    const float ch4_voltage = ch4_voltage_res.value();
    const float ch1_current = ch1_current_res.value();
    const float ch2_current = ch2_current_res.value();
    const float ch3_current = ch3_current_res.value();
    const float ch4_current = ch4_current_res.value();
    const float ch1_power = ch1_power_res.value();
    const float ch2_power = ch2_power_res.value();
    const float ch3_power = ch3_power_res.value();
    const float ch4_power = ch4_power_res.value();

    app::can_tx::VC_ChannelOneVoltage_set(ch1_voltage);
    app::can_tx::VC_ChannelTwoVoltage_set(ch2_voltage);
    app::can_tx::VC_ChannelThreeVoltage_set(ch3_voltage);
    app::can_tx::VC_ChannelFourVoltage_set(ch4_voltage);

    app::can_tx::VC_ChannelOneCurrent_set(ch1_current);
    app::can_tx::VC_ChannelTwoCurrent_set(ch2_current);
    app::can_tx::VC_ChannelThreeCurrent_set(ch3_current);
    app::can_tx::VC_ChannelFourCurrent_set(ch4_current);

    app::can_tx::VC_ChannelOnePower_set(ch1_power);
    app::can_tx::VC_ChannelTwoPower_set(ch2_power);
    app::can_tx::VC_ChannelThreePower_set(ch3_power);
    app::can_tx::VC_ChannelFourPower_set(ch4_power);
}

} // namespace app::powerMonitoring
