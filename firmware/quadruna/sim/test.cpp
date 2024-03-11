#include <gtest/gtest.h>

#include "VC.h"

TEST(test, test_basic)
{
    VC::init();
    VC::tick100Hz();
    ASSERT_TRUE(1 + 1 == 2);
}
