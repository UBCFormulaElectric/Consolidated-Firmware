#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_startSwitch.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCStartSwitchTest : public VCBaseTest
{
};

// Testing when theres a rising edge from off to on
TEST_F(VCStartSwitchTest, test_rising_edge_from_off_to_on) {
    ASSERT_FALSE(app::startSwitch::hasRisingEdge());
    fakes::io::canRx::set_CRIT_StartSwitch_state(app::can_utils::SwitchState::SWITCH_ON);
    ASSERT_TRUE(app::startSwitch::hasRisingEdge());
}

// Testing when theres not a rising edge from off to on
TEST_F(VCStartSwitchTest, test_no_rising_edge) {
    fakes::io::canRx::set_CRIT_StartSwitch_state(app::can_utils::SwitchState::SWITCH_ON);
    ASSERT_FALSE(app::startSwitch::hasRisingEdge());
}
