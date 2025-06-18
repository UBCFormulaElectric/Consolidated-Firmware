#include "test_VCBase.hpp"

extern "C"
{
    // TODO remove this is temporary
#include "io_canTx.h"
    void io_canTx_init(
        void (*transmit_can1_msg_func)(const JsonCanMsg *),
        void (*transmit_can2_msg_func)(const JsonCanMsg *),
        void (*transmit_can3_msg_func)(const JsonCanMsg *))
    {
        (void)transmit_can1_msg_func;
        (void)transmit_can2_msg_func;
        (void)transmit_can3_msg_func;
    }
}

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
#include "io_loadswitches.h"
}

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
    bool rl_pump_efuse;
    bool l_rad_fan_efuse;
    bool r_rad_fan_efuse;
};
#define check_efuses(                                                                                       \
    f_inv_efuse, rsm_efuse, bms_efuse, r_inv_efuse, dam_efuse, front_efuse, rl_pump_efuse, l_rad_fan_efuse, \
    r_rad_fan_efuse)                                                                                        \
    ASSERT_EQ(inv_rsm_loadswitch.efuse1->enabled, f_inv_efuse);                                             \
    ASSERT_EQ(inv_rsm_loadswitch.efuse2->enabled, rsm_efuse);                                               \
    ASSERT_EQ(inv_bms_loadswitch.efuse1->enabled, bms_efuse);                                               \
    ASSERT_EQ(inv_bms_loadswitch.efuse2->enabled, r_inv_efuse);                                             \
    ASSERT_EQ(front_loadswitch.efuse1->enabled, dam_efuse);                                                 \
    ASSERT_EQ(front_loadswitch.efuse2->enabled, front_efuse);                                               \
    ASSERT_EQ(rl_pump_loadswitch.efuse->enabled, rl_pump_efuse);                                            \
    ASSERT_EQ(rad_fan_loadswitch.efuse1->enabled, l_rad_fan_efuse);                                         \
    ASSERT_EQ(rad_fan_loadswitch.efuse2->enabled, r_rad_fan_efuse);

TEST_F(VCPowerManagerTest, test_sequencingStateMachine)
{
    // this tests that the efuse sequencing happens
    ASSERT_EQ(app_stateMachine_getCurrentState(), &init_state)
        << app_stateMachine_getCurrentState()->name << " != init_state";
    LetTimePass(10);
    check_efuses(false, true, true, false, true, true, false, false, false);

    // close ir negative
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &inverterOn_state)
        << app_stateMachine_getCurrentState()->name << " != inverterOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false);

    // turn on inverters
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &bmsOn_state)
        << app_stateMachine_getCurrentState()->name << " != bmsOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false);

    // turn on the bms
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &pcmOn_state)
        << app_stateMachine_getCurrentState()->name << " != pcmOn_state";
    check_efuses(true, true, true, true, true, true, false, false, false);

    // // todo whatever gets us out of PCM on state
    // LetTimePass(10);
    // ASSERT_EQ(app_stateMachine_getCurrentState(), &hvInit_state)
    //     << app_stateMachine_getCurrentState()->name << " != hvInit_state";
    // check_efuses({
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     false,
    //     false,
    //     false,
    //     false,
    // });
    // LetTimePass(200);
    // check_efuses({
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     false,
    //     false,
    //     false,
    // });
    // LetTimePass(200);
    // check_efuses({
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     false,
    //     false,
    // });
    // LetTimePass(200);
    // check_efuses({
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     false,
    // });
    // LetTimePass(200);
    // check_efuses({
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    //     true,
    // });
}

TEST_F(VCPowerManagerTest, test_steadyStateFailure)
{
    // this tests that in steady state, if an efuse fails, that the failure is handled
}

TEST_F(VCPowerManagerTest, test_startupFailure)
{
    // this tests that while starting up, if an efuse fails, that it is handled in order
}
