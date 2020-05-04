#pragma once

#include "Test_Dim.h"

class BinarySwitchTest : public virtual testing::Test
{
  public:
    void TearDownBinarySwitch(struct BinarySwitch *&binary_switch_to_teardown);

  protected:
    void                 SetUp() override;
    void                 TearDown() override;
    struct BinarySwitch *binary_switch;
};
