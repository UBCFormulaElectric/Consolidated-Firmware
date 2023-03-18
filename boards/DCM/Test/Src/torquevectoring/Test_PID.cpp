

#include "Test_Dcm.h"

extern "C"
{
    #include "torquevectoring/App_PID.h"
    #include "torquevectoring/App_TorqueVectoring.h"
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


TEST_F(PIDTest, pid_config_test1){
    
}