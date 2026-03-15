#include "test_VCBase.hpp"
#include <gtest/gtest.h>
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include "app_faultHandling.cpp"
#include "app_canTx.hpp"

class VCTestAppFaultHandling : public VCBaseTest
{
};
TEST_F(VCTestAppFaultHandling, airMinusTest)
{
    // Arrange
    app::can_rx::BMS_IrNegative_update(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(1); // Assuming it takes some time for the contactor state to update after the start switch is turned on

    // Assert
    EXPECT_TRUE(app::fault_handling::air_minus_closed());
}

TEST_F(VCTestAppFaultHandling, fsmBmsHeartbeartChecksTest)
{
    // No fault
    app::can_tx::VC_Fault_InvLockoutFault_set(true);
    LetTimePass(1);
    EXPECT_TRUE(app::fault_handling::HeartbeartChecks());

    // FAULTED
    app::can_tx::VC_Fault_InvLockoutFault_set(false);
    LetTimePass(1);
    EXPECT_FALSE(app::fault_handling::HeartbeartChecks());
}