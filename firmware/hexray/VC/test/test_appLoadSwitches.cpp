#include "test_VCBase.hpp"
#include <gtest/gtest.h>
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include "app_loadSwitches.cpp"
#include "app_canTx.hpp"
////// temporary ///
class VCTestAppLoadSwitches : public VCBaseTest
{
};

// F_TEST(VCTestAppLoadSwitches, testefuseBroadcase){
// app::loadswitches::efuse_broadcast();
// }