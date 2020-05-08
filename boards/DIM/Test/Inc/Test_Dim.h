#pragma once

// The argument history needs to be long enough for us to test write every
// one of the possible 16x16x16 = 4096 values on the three 7-segment displays.
#include <math.h>
#include "App_SharedConstants.h"
#define FFF_ARG_HISTORY_LEN (size_t) pow((float)NUM_HEX_DIGITS, (float)3)

#include <fff.h>
#include <gtest/gtest.h>

#include "Test_Utils.h"
