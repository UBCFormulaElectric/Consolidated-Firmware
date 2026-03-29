#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_shdnLast.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCShdnLastTest : public VCBaseTest
{
};

// Testing broadcating when each shutdown latch is not ok in order
TEST_F(VCShdnLastTest, test_latch_not_ok_in_order) {}
