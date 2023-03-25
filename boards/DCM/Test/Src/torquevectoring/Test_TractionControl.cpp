

#include "Test_Dcm.h"

extern "C"
{
    #include "torquevectoring/App_TorqueVectoring.h"
    #include "torquevectoring/App_TractionControl.h"
}

class TractionControlTest : public testing::Test
{
    protected:
        void SetUp() override
        {
            return;
        }
        void TearDown() override
        {
            return;
        }
};  

TEST(TractionControlTest, TractionControl_init)
{
    PID_Config tc_pid_config = { 0.0, 0.0, 0.0, 0.0, 10.0};
    PID tc_pid;
    App_PID_Init(&tc_pid, &tc_pid_config);
    TractionControl_Inputs test_inputs = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &tc_pid };
    TractionControl_Outputs test_outputs = { 0.0, 0.0 };
    App_TractionControl_ComputeTorque(&test_inputs, &test_outputs);
    ASSERT_NEAR(test_outputs.torque_left_final_Nm, 0.0, 0.000001);
    ASSERT_NEAR(test_outputs.torque_right_final_Nm, 0.0, 0.000001);
}