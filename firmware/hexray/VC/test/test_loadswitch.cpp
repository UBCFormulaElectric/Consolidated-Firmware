#include <gtest/gtest.h>

#include "app_canTx.hpp"
#include "app_powerManager.hpp"
#include "io_efuses.hpp"
#include "test/test_VCBase.hpp"
#include "vc_fakes.hpp"

class VCLoadSwitchTest : public VCBaseTest
{
};

static constexpr app::powerManager::Efuses<app::powerManager::EfuseConfig> enabled_config = {
    .front_efuse     = { true, 0, 5 },
    .rsm_efuse       = { true, 0, 5 },
    .bms_efuse       = { true, 0, 5 },
    .dam_efuse       = { true, 0, 5 },
    .f_inv_efuse     = { true, 0, 5 },
    .r_inv_efuse     = { true, 0, 5 },
    .r_rad_fan_efuse = { true, 0, 5 },
    .l_rad_fan_efuse = { true, 0, 5 },
    .rr_pump_efuse   = { true, 0, 5 },
    .rl_pump_efuse   = { true, 0, 5 },
};

TEST_F(VCLoadSwitchTest, load_switch_broadcast_on)
{
    app::powerManager::updateConfig(enabled_config);

    f_inv_efuse.setChannel(true);
    f_inv_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_FrontInvertersStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_FrontInvertersCurrent_get());

    r_inv_efuse.setChannel(true);
    r_inv_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_RearInvertersStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearInvertersCurrent_get());

    r_rad_fan_efuse.setChannel(true);
    r_rad_fan_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_RightRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RightRadiatorFanCurrent_get());

    l_rad_fan_efuse.setChannel(true);
    l_rad_fan_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_LeftRadiatorFanStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_LeftRadiatorFanCurrent_get());

    bms_efuse.setChannel(true);
    bms_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_BMSStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_BMSCurrent_get());

    dam_efuse.setChannel(true);
    dam_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_DAMStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_DAMCurrent_get());

    front_efuse.setChannel(true);
    front_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_FrontStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_FrontCurrent_get());

    rsm_efuse.setChannel(true);
    rsm_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_RSMStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RSMCurrent_get());

    rl_pump_efuse.setChannel(true);
    rl_pump_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_RearLeftPumpStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearLeftPumpCurrent_get());

    rr_pump_efuse.setChannel(true);
    rr_pump_efuse.setChannelCurrent(1.23f);
    app::powerManager::efuseProtocolTick_100Hz();
    ASSERT_TRUE(app::can_tx::VC_RearRightPumpStatus_get());
    ASSERT_FLOAT_EQ(1.23f, app::can_tx::VC_RearRightPumpCurrent_get());
}