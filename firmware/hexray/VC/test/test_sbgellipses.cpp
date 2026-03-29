#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_sbgEllipses.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCSbgEllipsesTest : public VCBaseTest
{
};

// Testing broadcating
TEST_F(VCSbgEllipsesTest, test_rl_pump_not_ready) {}

// Testing getting vehicle velocity
TEST_F(VCSbgEllipsesTest, test_get_vehicle_velocity) {}

// Testing getting ekf solution mode
TEST_F(VCSbgEllipsesTest, test_get_ekf_solution_mode) {}
