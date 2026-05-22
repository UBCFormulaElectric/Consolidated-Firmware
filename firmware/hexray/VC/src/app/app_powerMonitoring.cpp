#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
#include "app_canTx.hpp"

namespace app::powerMonitoring
{

result<void> update()
{
    static bool init_done = false;

    for (int tries = 0; not init_done and tries < 10; tries++)
    {
        init_done = io::powerMonitoring::init().has_value();
    }
    if (not init_done)
    {
        // error handle
        LOG_ERROR("Power monitoring initialization failed");
        return std::unexpected(ErrorCode::ERROR);
    }

    RETURN_IF_ERR(io::powerMonitoring::monitor_power_inputs());
    RETURN_IF_ERR(io::powerMonitoring::refresh());

    const auto ch1_voltage = io::powerMonitoring::read_voltage(Channel::CH1);
    const auto ch2_voltage = io::powerMonitoring::read_voltage(Channel::CH2);
    const auto ch3_voltage = io::powerMonitoring::read_voltage(Channel::CH3);
    const auto ch4_voltage = io::powerMonitoring::read_voltage(Channel::CH4);

    const auto ch1_current = io::powerMonitoring::read_current(Channel::CH1);
    const auto ch2_current = io::powerMonitoring::read_current(Channel::CH2);
    const auto ch3_current = io::powerMonitoring::read_current(Channel::CH3);
    const auto ch4_current = io::powerMonitoring::read_current(Channel::CH4);

    const auto ch1_power = io::powerMonitoring::read_power(Channel::CH1);
    const auto ch2_power = io::powerMonitoring::read_power(Channel::CH2);
    const auto ch3_power = io::powerMonitoring::read_power(Channel::CH3);
    const auto ch4_power = io::powerMonitoring::read_power(Channel::CH4);

    const bool ch1_voltage_valid = ch1_voltage.has_value();
    const bool ch2_voltage_valid = ch2_voltage.has_value();
    const bool ch3_voltage_valid = ch3_voltage.has_value();
    const bool ch4_voltage_valid = ch4_voltage.has_value();

    const bool ch1_current_valid = ch1_current.has_value();
    const bool ch2_current_valid = ch2_current.has_value();
    const bool ch3_current_valid = ch3_current.has_value();
    const bool ch4_current_valid = ch4_current.has_value();

    const bool ch1_power_valid = ch1_power.has_value();
    const bool ch2_power_valid = ch2_power.has_value();
    const bool ch3_power_valid = ch3_power.has_value();
    const bool ch4_power_valid = ch4_power.has_value();

    app::can_tx::VC_CHANNEL1_VOLTAGE_VALID_set(ch1_voltage_valid);
    app::can_tx::VC_CHANNEL2_VOLTAGE_VALID_set(ch2_voltage_valid);
    app::can_tx::VC_CHANNEL3_VOLTAGE_VALID_set(ch3_voltage_valid);
    app::can_tx::VC_CHANNEL4_VOLTAGE_VALID_set(ch4_voltage_valid);

    app::can_tx::VC_CHANNEL1_CURRENT_VALID_set(ch1_current_valid);
    app::can_tx::VC_CHANNEL2_CURRENT_VALID_set(ch2_current_valid);
    app::can_tx::VC_CHANNEL3_CURRENT_VALID_set(ch3_current_valid);
    app::can_tx::VC_CHANNEL4_CURRENT_VALID_set(ch4_current_valid);

    app::can_tx::VC_CHANNEL1_POWER_VALID_set(ch1_power_valid);
    app::can_tx::VC_CHANNEL2_POWER_VALID_set(ch2_power_valid);
    app::can_tx::VC_CHANNEL3_POWER_VALID_set(ch3_power_valid);
    app::can_tx::VC_CHANNEL4_POWER_VALID_set(ch4_power_valid);

    app::can_tx::VC_ChannelOneVoltage_set(ch1_voltage.value_or(0.0f));
    app::can_tx::VC_ChannelTwoVoltage_set(ch2_voltage.value_or(0.0f));
    app::can_tx::VC_ChannelThreeVoltage_set(ch3_voltage.value_or(0.0f));
    app::can_tx::VC_ChannelFourVoltage_set(ch4_voltage.value_or(0.0f));

    app::can_tx::VC_ChannelOneCurrent_set(ch1_current.value_or(0.0f));
    app::can_tx::VC_ChannelTwoCurrent_set(ch2_current.value_or(0.0f));
    app::can_tx::VC_ChannelThreeCurrent_set(ch3_current.value_or(0.0f));
    app::can_tx::VC_ChannelFourCurrent_set(ch4_current.value_or(0.0f));

    app::can_tx::VC_ChannelOnePower_set(ch1_power.value_or(0.0f));
    app::can_tx::VC_ChannelTwoPower_set(ch2_power.value_or(0.0f));
    app::can_tx::VC_ChannelThreePower_set(ch3_power.value_or(0.0f));
    app::can_tx::VC_ChannelFourPower_set(ch4_power.value_or(0.0f));

    uint8_t    OV_UV_mask   = 0u;
    const auto alert_status = io::powerMonitoring::read_alert_status();
    if (alert_status.has_value())
    {
        OV_UV_mask = alert_status.value();
    }

    app::can_tx::VC_CHANNEL1_OV_set((OV_UV_mask & ALERT_OV_CH1) != 0u);
    app::can_tx::VC_CHANNEL2_OV_set((OV_UV_mask & ALERT_OV_CH2) != 0u);
    app::can_tx::VC_CHANNEL3_OV_set((OV_UV_mask & ALERT_OV_CH3) != 0u);
    app::can_tx::VC_CHANNEL4_OV_set((OV_UV_mask & ALERT_OV_CH4) != 0u);

    app::can_tx::VC_CHANNEL1_UV_set((OV_UV_mask & ALERT_UV_CH1) != 0u);
    app::can_tx::VC_CHANNEL2_UV_set((OV_UV_mask & ALERT_UV_CH2) != 0u);
    app::can_tx::VC_CHANNEL3_UV_set((OV_UV_mask & ALERT_UV_CH3) != 0u);
    app::can_tx::VC_CHANNEL4_UV_set((OV_UV_mask & ALERT_UV_CH4) != 0u);

    return {};
}
} // namespace app::powerMonitoring
