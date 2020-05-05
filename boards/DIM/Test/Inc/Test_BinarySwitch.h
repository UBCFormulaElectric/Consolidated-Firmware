#pragma once

#include "Test_Dim.h"

class BinarySwitchTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;
    void TearDownBinarySwitch(struct BinarySwitch *&binary_switch_to_teardown);
    struct BinarySwitch *binary_switch;
};
