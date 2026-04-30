#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
#include "io_powerMonitoring_datatypes.hpp"
#include "app_canTx.hpp"

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

    if (io::powerMonitoring::is_alert_asserted())
    {
        std::expected<uint8_t, ErrorCode> alert_status = io::powerMonitoring::read_alert_status();
        if (alert_status.has_value())
        {
            const uint8_t OV_UV_mask = alert_status.value();

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

    const auto ch1_voltage = io::powerMonitoring::read_voltage(CH1);
    const auto ch2_voltage = io::powerMonitoring::read_voltage(CH2);
    const auto ch3_voltage = io::powerMonitoring::read_voltage(CH3);
    const auto ch4_voltage = io::powerMonitoring::read_voltage(CH4);

    const auto ch1_current = io::powerMonitoring::read_current(CH1);
    const auto ch2_current = io::powerMonitoring::read_current(CH2);
    const auto ch3_current = io::powerMonitoring::read_current(CH3);
    const auto ch4_current = io::powerMonitoring::read_current(CH4);

    const auto ch1_power = io::powerMonitoring::read_power(CH1);
    const auto ch2_power = io::powerMonitoring::read_power(CH2);
    const auto ch3_power = io::powerMonitoring::read_power(CH3);
    const auto ch4_power = io::powerMonitoring::read_power(CH4);

    if (!ch1_voltage.has_value() || !ch2_voltage.has_value() || !ch3_voltage.has_value() || !ch4_voltage.has_value() ||
        !ch1_current.has_value() || !ch2_current.has_value() || !ch3_current.has_value() || !ch4_current.has_value() ||
        !ch1_power.has_value() || !ch2_power.has_value() || !ch3_power.has_value() || !ch4_power.has_value())
    {
        return;
    }

    app::can_tx::VC_ChannelOneVoltage_set(ch1_voltage.value());
    app::can_tx::VC_ChannelTwoVoltage_set(ch2_voltage.value());
    app::can_tx::VC_ChannelThreeVoltage_set(ch3_voltage.value());
    app::can_tx::VC_ChannelFourVoltage_set(ch4_voltage.value());

    app::can_tx::VC_ChannelOneCurrent_set(ch1_current.value());
    app::can_tx::VC_ChannelTwoCurrent_set(ch2_current.value());
    app::can_tx::VC_ChannelThreeCurrent_set(ch3_current.value());
    app::can_tx::VC_ChannelFourCurrent_set(ch4_current.value());

    app::can_tx::VC_ChannelOnePower_set(ch1_power.value());
    app::can_tx::VC_ChannelTwoPower_set(ch2_power.value());
    app::can_tx::VC_ChannelThreePower_set(ch3_power.value());
    app::can_tx::VC_ChannelFourPower_set(ch4_power.value());
}

} // namespace app::powerMonitoring
