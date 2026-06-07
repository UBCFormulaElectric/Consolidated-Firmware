#include "app_powerMonitoring.hpp"
#include "io_powerMonitoring.hpp"
#include "io_semaphore.hpp"
#include "app_canTx.hpp"

namespace 
{
io::semaphore i2c_bus{ true }; // protects i2c_bus_4
}

namespace app::powerMonitoring
{

result<void> update()
{
    const io::unique_semaphore lock{ i2c_bus };
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

    const auto ch_boost_voltage = io::powerMonitoring::read_voltage(Channel::CH1);
    const auto ch_ext_voltage   = io::powerMonitoring::read_voltage(Channel::CH2);
    const auto ch_vbat_voltage  = io::powerMonitoring::read_voltage(Channel::CH3);
    const auto ch_pcm_voltage   = io::powerMonitoring::read_voltage(Channel::CH4);

    const auto ch_boost_current = io::powerMonitoring::read_current(Channel::CH1);
    const auto ch_ext_current   = io::powerMonitoring::read_current(Channel::CH2);
    const auto ch_vbat_current  = io::powerMonitoring::read_current(Channel::CH3);
    const auto ch_pcm_current   = io::powerMonitoring::read_current(Channel::CH4);

    const auto ch_boost_power = io::powerMonitoring::read_power(Channel::CH1);
    const auto ch_ext_power   = io::powerMonitoring::read_power(Channel::CH2);
    const auto ch_vbat_power  = io::powerMonitoring::read_power(Channel::CH3);
    const auto ch_pcm_power   = io::powerMonitoring::read_power(Channel::CH4);

    

    const bool ch_pcm_voltage_valid   = ch_pcm_voltage.has_value();
    const bool ch_ext_voltage_valid   = ch_ext_voltage.has_value();
    const bool ch_vbat_voltage_valid  = ch_vbat_voltage.has_value();
    const bool ch_boost_voltage_valid = ch_boost_voltage.has_value();

    const bool ch_pcm_current_valid   = ch_pcm_current.has_value();
    const bool ch_ext_current_valid   = ch_ext_current.has_value();
    const bool ch_vbat_current_valid  = ch_vbat_current.has_value();
    const bool ch_boost_current_valid = ch_boost_current.has_value();

    const bool ch_pcm_power_valid   = ch_pcm_power.has_value();
    const bool ch_ext_power_valid   = ch_ext_power.has_value();
    const bool ch_vbat_power_valid  = ch_vbat_power.has_value();
    const bool ch_boost_power_valid = ch_boost_power.has_value();

    app::can_tx::VC_PCM_CHANNEL_VOLTAGE_VALID_set(ch_pcm_voltage_valid);
    app::can_tx::VC_EXT_CHANNEL_VOLTAGE_VALID_set(ch_ext_voltage_valid);
    app::can_tx::VC_VBAT_CHANNEL_VOLTAGE_VALID_set(ch_vbat_voltage_valid);
    app::can_tx::VC_BOOST_CHANNEL_VOLTAGE_VALID_set(ch_boost_voltage_valid);

    app::can_tx::VC_PCM_CHANNEL_CURRENT_VALID_set(ch_pcm_current_valid);
    app::can_tx::VC_EXT_CHANNEL_CURRENT_VALID_set(ch_ext_current_valid);
    app::can_tx::VC_VBAT_CHANNEL_CURRENT_VALID_set(ch_vbat_current_valid);
    app::can_tx::VC_BOOST_CHANNEL_CURRENT_VALID_set(ch_boost_current_valid);

    app::can_tx::VC_CHANNEL1_POWER_VALID_set(ch_boost_power_valid);
    app::can_tx::VC_CHANNEL2_POWER_VALID_set(ch_ext_power_valid);
    app::can_tx::VC_CHANNEL3_POWER_VALID_set(ch_vbat_power_valid);
    app::can_tx::VC_CHANNEL4_POWER_VALID_set(ch_pcm_power_valid);

    app::can_tx::VC_PcmChannelVoltage_set(ch_pcm_voltage.value_or(0.0f));
    app::can_tx::VC_ExtChannelVoltage_set(ch_ext_voltage.value_or(0.0f));
    app::can_tx::VC_VbatChannelVoltage_set(ch_vbat_voltage.value_or(0.0f));
    app::can_tx::VC_BoostChannelVoltage_set(ch_boost_voltage.value_or(0.0f));

    app::can_tx::VC_PcmChannelCurrent_set(ch_pcm_current.value_or(0.0f));
    app::can_tx::VC_ExtChannelCurrent_set(ch_ext_current.value_or(0.0f));
    app::can_tx::VC_VbatChannelCurrent_set(ch_vbat_current.value_or(0.0f));
    app::can_tx::VC_BoostChannelCurrent_set(ch_boost_current.value_or(0.0f));

    app::can_tx::VC_PcmChannelPower_set(ch_pcm_power.value_or(0.0f));
    app::can_tx::VC_ExtChannelPower_set(ch_ext_power.value_or(0.0f));
    app::can_tx::VC_VbatChannelPower_set(ch_vbat_power.value_or(0.0f));
    app::can_tx::VC_BoostChannelPower_set(ch_boost_power.value_or(0.0f));

    AlertOvUvBits ov_uv{};
    const auto    alert_status = io::powerMonitoring::read_alert_status();
    if (alert_status.has_value())
    {
        ov_uv.alert_status = alert_status.value();
    }

    app::can_tx::VC_CHANNEL1_BOOST_OV_set(ov_uv.bits.CH1OV);
    app::can_tx::VC_CHANNEL2_EXT_OV_set(ov_uv.bits.CH2OV);
    app::can_tx::VC_CHANNEL3_VBAT_OV_set(ov_uv.bits.CH3OV);
    app::can_tx::VC_CHANNEL4_PCM_OV_set(ov_uv.bits.CH4OV);

    app::can_tx::VC_CHANNEL1_BOOST_UV_set(ov_uv.bits.CH1UV);
    app::can_tx::VC_CHANNEL2_EXT_UV_set(ov_uv.bits.CH2UV);
    app::can_tx::VC_CHANNEL3_VBAT_UV_set(ov_uv.bits.CH3UV);
    app::can_tx::VC_CHANNEL4_PCM_UV_set(ov_uv.bits.CH4UV);

    return {};
}
} // namespace app::powerMonitoring
