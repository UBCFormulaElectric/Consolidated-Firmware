/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 *
 *
 *
*/

#include "Test_Dcm.h"

extern "C"
{
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "math.h"
}
/**
 * Define SetUp and TearDown to be called before every TEST_F call
*/
class ActiveDifferentialTest: public testing::Test
{
protected:
    //TODO: Find out if this is necessary if no IO functions are called.
    void SetUp() override
    {
        return;
    }
    void TearDown() override
    {
        return;
    }
};

TEST_F(ActiveDifferentialTest, test_1){

}