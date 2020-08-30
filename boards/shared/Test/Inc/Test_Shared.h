#pragma once

#include <fff.h>
#include <gtest/gtest.h>

#include "Test_Utils.h"

DECLARE_FAKE_VALUE_FUNC(bool, is_high, struct World *);
DECLARE_FAKE_VOID_FUNC(callback_function, struct World *);
