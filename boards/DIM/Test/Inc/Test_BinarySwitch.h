#pragma once

#include "Test_Dim.h"

class BinarySwitchTest : public virtual testing::Test
{
  public:
    struct BinarySwitch *SetUpBinarySwitch(bool (*is_turned_on)(void));
    void TearDownBinarySwitch(struct BinarySwitch *&binary_switch);

  protected:
    void                 SetUp() override;
    void                 TearDown() override;
    struct BinarySwitch *_binary_switch;
};
