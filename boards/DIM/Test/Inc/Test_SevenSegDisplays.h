#pragma once

#include "Test_Dim.h"

extern "C"
{
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_ErrorCode.h"
}

DECLARE_FAKE_VOID_FUNC(set_right_hex_digit, struct SevenSegHexDigit);
DECLARE_FAKE_VOID_FUNC(set_middle_hex_digit, struct SevenSegHexDigit);
DECLARE_FAKE_VOID_FUNC(set_left_hex_digit, struct SevenSegHexDigit);
