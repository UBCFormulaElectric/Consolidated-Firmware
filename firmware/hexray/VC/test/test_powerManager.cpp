#include "test_VCBase.hpp"

#include "states/app_states.hpp"
#include "app_canRx.hpp"
#include "io_efuses.hpp"
#include "app_canUtils.hpp"
#include "vc_fakes.hpp"

class VCPowerManagerTest : public VCBaseTest
{
};

struct efuse_expected_state
{
    bool f_inv_efuse;
    bool rsm_efuse;
    bool bms_efuse;
    bool r_inv_efuse;
    bool dam_efuse;
    bool front_efuse;
    bool rr_pump_efuse;
    bool rl_pump_efuse;
    bool l_rad_fan_efuse;
    bool r_rad_fan_efuse;
};

#define check_efuses(                                                                                           \
    f_inv_expected, rsm_expected, bms_expected, r_inv_expected, dam_expected, front_expected, rr_pump_expected, \
    rl_pump_expected, l_rad_fan_expected, r_rad_fan_expected)                                                   \
    ASSERT_EQ(f_inv_efuse.isChannelEnabled(), f_inv_expected);                                                  \
    ASSERT_EQ(rsm_efuse.isChannelEnabled(), rsm_expected);                                                      \
    ASSERT_EQ(bms_efuse.isChannelEnabled(), bms_expected);                                                      \
    ASSERT_EQ(r_inv_efuse.isChannelEnabled(), r_inv_expected);                                                  \
    ASSERT_EQ(dam_efuse.isChannelEnabled(), dam_expected);                                                      \
    ASSERT_EQ(front_efuse.isChannelEnabled(), front_expected);                                                  \
    ASSERT_EQ(rr_pump_efuse.isChannelEnabled(), rr_pump_expected);                                              \
    ASSERT_EQ(rl_pump_efuse.isChannelEnabled(), rl_pump_expected);                                              \
    ASSERT_EQ(l_rad_fan_efuse.isChannelEnabled(), l_rad_fan_expected);                                          \
    ASSERT_EQ(r_rad_fan_efuse.isChannelEnabled(), r_rad_fan_expected);

using namespace app::can_utils;

TEST_F(VCPowerManagerTest, test_sequencingStateMachine)
{
    // this tests that the efuse sequencing happens
    ASSERT_EQ(app::StateMachine::get_current_state(), &app::states::init_state)
        << app::StateMachine::get_current_state()->name << " != init_state";
    LetTimePass(10);
    check_efuses(false, true, true, false, true, true, false, false, false, false);

    // close ir negative
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_EQ(app::StateMachine::get_current_state(), &app::states::inverterOn_state)
        << app::StateMachine::get_current_state()->name << " != inverterOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false, false);

    // turn on inverters
    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);

    LetTimePass(10);
    ASSERT_EQ(app::StateMachine::get_current_state(), &app::states::bmsOn_state)
        << app::StateMachine::get_current_state()->name << " != bmsOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false, false);

    // turn on the bms
    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_EQ(app::StateMachine::get_current_state(), &app::states::pcmOn_state)
        << app::StateMachine::get_current_state()->name << " != pcmOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false, false);

    // todo whatever gets us out of PCM on state
    LetTimePass(10);
    ASSERT_EQ(app::StateMachine::get_current_state(), &app::states::hvInit_state)
        << app::StateMachine::get_current_state()->name << " != hvInit_state";
}
