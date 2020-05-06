#pragma once

#include "Test_Dim.h"

class RegenPaddleTest : public virtual testing::Test
{
  protected:
    void SetUp() override;
    void TearDown() override;

    const uint32_t      DEFAULT_LOWER_DEADZONE = 5;
    const uint32_t      DEFAULT_UPPER_DEADZONE = 95;
    struct RegenPaddle *regen_paddle;
};
