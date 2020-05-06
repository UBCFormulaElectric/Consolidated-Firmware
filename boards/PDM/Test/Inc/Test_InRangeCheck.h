#pragma once

#include "Test_Pdm.h"

class InRangeCheckTest : public virtual testing::Test
{
  protected:
    void TearDownInRangeCheck(struct InRangeCheck *&in_range_check_to_teardown);
    void SetUp() override;
    void TearDown() override;

    struct InRangeCheck *in_range_check;

    const float DEFAULT_IN_RANGE_CHECK_MIN_VALUE = 5.0f;
    const float DEFAULT_IN_RANGE_CHECK_MAX_VALUE = 6.0f;
};
