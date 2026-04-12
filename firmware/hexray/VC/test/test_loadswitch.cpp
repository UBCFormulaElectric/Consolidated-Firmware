#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_loadswitches.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "io_timeFake.hpp"

class VCLoadSwitchTest : public VCBaseTest
{
};

// test braodcasting of channels
TEST_F(VCLoadSwitchTest, load_switch_broadcast_on)
{
    // f_inv_efuse on and off
    f_inv_efuse.setChannel(true);
    f_inv_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_FrontInvertersStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_FrontInvertersCurrent_get());

    f_inv_efuse.setChannel(false);
    f_inv_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_FrontInvertersStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_FrontInvertersCurrent_get());

    // rsm_efuse on and off
    r_inv_efuse.setChannel(true);
    r_inv_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_RSMStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RSMCurrent_get());

    r_inv_efuse.setChannel(false);
    r_inv_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_RSMStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_RSMCurrent_get());

    // bms_efuse on and off
    bms_efuse.setChannel(true);
    bms_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_BMSStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_BMSCurrent_get());

    bms_efuse.setChannel(false);
    bms_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_BMSStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_BMSCurrent_get());

    // rsm_efuse on and off
    rsm_efuse.setChannel(true);
    rsm_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_RearInvertersStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearInvertersCurrent_get());

    rsm_efuse.setChannel(false);
    rsm_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_RearInvertersStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_RearInvertersCurrent_get());

    // dam efuse on and off
    dam_efuse.setChannel(true);
    dam_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_DAMStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_DAMCurrent_get());

    dam_efuse.setChannel(false);
    dam_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_DAMStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_DAMCurrent_get());

    // front_efuse on and off
    front_efuse.setChannel(true);
    front_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_FrontStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_FrontCurrent_get());

    front_efuse.setChannel(false);
    front_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_FrontStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_FrontCurrent_get());

    // l_rad_fan_efuse on and off
    l_rad_fan_efuse.setChannel(true);
    l_rad_fan_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_RearLeftPumpStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearLeftPumpCurrent_get());

    l_rad_fan_efuse.setChannel(false);
    l_rad_fan_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_RearLeftPumpStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_RearLeftPumpCurrent_get());

    // r_rad_fan_efuse on and off
    r_rad_fan_efuse.setChannel(true);
    r_rad_fan_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_RightRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RightRadiatorFanCurrent_get());

    r_rad_fan_efuse.setChannel(false);
    r_rad_fan_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_RightRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_RightRadiatorFanCurrent_get());

    // rr_pump_efuse on and off
    rr_pump_efuse.setChannel(true);
    rr_pump_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_RearRightPumpStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearRightPumpCurrent_get());

    rr_pump_efuse.setChannel(false);
    rr_pump_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_RearRightPumpStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_RearRightPumpCurrent_get());

    // rl_pump_efuse on and off
    rl_pump_efuse.setChannel(true);
    rl_pump_efuse.setChannelCurrent(1.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_TRUE(app::can_tx::VC_LeftRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_LeftRadiatorFanCurrent_get());

    rl_pump_efuse.setChannel(false);
    rl_pump_efuse.setChannelCurrent(0.23f);
    app::loadswitches::efuse_broadcast();
    ASSERT_FALSE(app::can_tx::VC_LeftRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(0.23f, app::can_tx::VC_LeftRadiatorFanCurrent_get());
}
