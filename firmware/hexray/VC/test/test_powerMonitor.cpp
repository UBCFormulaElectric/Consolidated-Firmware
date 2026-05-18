#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_powerMonitoring.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCPowerMonitorTest : public VCBaseTest
{
};

TEST_F(VCPowerMonitorTest, updating_test)
{
    fakes::io::powerMonitoring::set_reading_voltage(CH1, 5.0f);
    fakes::io::powerMonitoring::set_reading_current(CH1, 2.0f);
    fakes::io::powerMonitoring::set_reading_power(CH1, 10.0f);
    fakes::io::powerMonitoring::set_reading_voltage(CH2, 6.0f);
    fakes::io::powerMonitoring::set_reading_current(CH2, 3.0f);
    fakes::io::powerMonitoring::set_reading_power(CH2, 18.0f);
    fakes::io::powerMonitoring::set_reading_voltage(CH3, 7.0f);
    fakes::io::powerMonitoring::set_reading_current(CH3, 4.0f);
    fakes::io::powerMonitoring::set_reading_power(CH3, 28.0f);
    fakes::io::powerMonitoring::set_reading_voltage(CH4, 8.0f);
    fakes::io::powerMonitoring::set_reading_current(CH4, 5.0f);
    fakes::io::powerMonitoring::set_reading_power(CH4, 40.0f);

    app::powerMonitoring::update();

    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_ChannelOneVoltage_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_ChannelOneCurrent_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::VC_ChannelOnePower_get());
    ASSERT_FLOAT_EQ(6.0f, app::can_tx::VC_ChannelTwoVoltage_get());
    ASSERT_FLOAT_EQ(3.0f, app::can_tx::VC_ChannelTwoCurrent_get());
    ASSERT_FLOAT_EQ(18.0f, app::can_tx::VC_ChannelTwoPower_get());
    ASSERT_FLOAT_EQ(7.0f, app::can_tx::VC_ChannelThreeVoltage_get());
    ASSERT_FLOAT_EQ(4.0f, app::can_tx::VC_ChannelThreeCurrent_get());
    ASSERT_FLOAT_EQ(28.0f, app::can_tx::VC_ChannelThreePower_get());
    ASSERT_FLOAT_EQ(8.0f, app::can_tx::VC_ChannelFourVoltage_get());
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_ChannelFourCurrent_get());
    ASSERT_FLOAT_EQ(40.0f, app::can_tx::VC_ChannelFourPower_get());
}