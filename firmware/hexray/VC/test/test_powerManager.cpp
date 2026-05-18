#include "test_VCBase.hpp"

#include "states/app_states.hpp"
#include "app_canRx.hpp"
#include "io_efuses.hpp"
#include "app_canUtils.hpp"
#include "vc_fakes.hpp"
#include <app_canTx.hpp>

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
    rr_pump_expected, rl_pump_expected, l_rad_fan_expected,                                                     \
    r_rad_fan_expected, f_inv_expected, r_inv_expected, rsm_expected,                                           \
    bms_expected, dam_expected, front_expected)                                                                 \
    ASSERT_EQ(rr_pump_efuse.isChannelEnabled(), rr_pump_expected);                                              \
    ASSERT_EQ(rl_pump_efuse.isChannelEnabled(), rl_pump_expected);                                              \
    ASSERT_EQ(l_rad_fan_efuse.isChannelEnabled(), l_rad_fan_expected);                                          \
    ASSERT_EQ(r_rad_fan_efuse.isChannelEnabled(), r_rad_fan_expected);                                          \
    ASSERT_EQ(f_inv_efuse.isChannelEnabled(), f_inv_expected);                                                  \
    ASSERT_EQ(r_inv_efuse.isChannelEnabled(), r_inv_expected);                                                  \
    ASSERT_EQ(rsm_efuse.isChannelEnabled(), rsm_expected);                                                      \
    ASSERT_EQ(bms_efuse.isChannelEnabled(), bms_expected);                                                      \
    ASSERT_EQ(dam_efuse.isChannelEnabled(), dam_expected);                                                      \
    ASSERT_EQ(front_efuse.isChannelEnabled(), front_expected);                                                  \

using namespace app::can_utils;


// Helper to set state and invoke its entry action
static void SetStateWithEntry(const app::State *s)
{
    app::StateMachine::set_current_state(s);
    s->run_on_entry();
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_OPEN);
}

TEST_F(VCPowerManagerTest, test_sequencingStateMachine)
{
    // this tests that the efuse sequencing happens
    SetStateWithEntry(&app::states::init_state);
    ASSERT_STATE_EQ(app::states::init_state);

    LetTimePass(100);
    check_efuses(false, false, false, false, false, false, true, true, true, true);

    // close ir negative
    app::can_rx::BMS_IrNegative_update(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::inverterOn_state);
    check_efuses(false, false, false, false, true, true, true, true, true, true);

    // turn on inverters
    app::can_rx::INVFL_bSystemReady_update(true);
    app::can_rx::INVFR_bSystemReady_update(true);
    app::can_rx::INVRL_bSystemReady_update(true);
    app::can_rx::INVRR_bSystemReady_update(true);

    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::bmsOn_state);
    check_efuses(false, false, false, false, true, true, true, true, true, true);

    // turn on the bms
    app::can_rx::BMS_State_update(BmsState::BMS_DRIVE_STATE);
    LetTimePass(100);
    ASSERT_STATE_EQ(app::states::pcmOn_state);
    check_efuses(false, false, false, false, true, true, true, true, true, true);

    // todo whatever gets us out of PCM on state
    app::can_tx::VC_ChannelOneVoltage_set(24.0f);
    LetTimePass(1000);
    ASSERT_STATE_EQ(app::states::hvInit_state);
    check_efuses(true, true, true, true, true, true, true, true, true, true);


}
