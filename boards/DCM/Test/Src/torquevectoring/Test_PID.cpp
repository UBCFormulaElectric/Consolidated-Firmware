

#include "Test_Dcm.h"

extern "C"
{
    #include "App_PID.h"
    #include "App_TorqueVectoring.h"
}

class PIDTest : public testing::Test
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

PID_Config* pid_config_test1;
pid_config_test1->Kp = 1.0;
pid_config_test1->Ki = 0.0;
pid_config_test1->Kd = 0.0;
pid_config_test1->out_min = 10.0;
pid_config_test1->out_min = 0.0;

TEST_F(PIDTest, pid_config_test1)