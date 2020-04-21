#include "Test_Dim.h"

extern "C"
{
#include "App_SocDigits.h"
}

class SocDigitsTest : public DimTest
{
    // Empty class because we just re-use the Setup() and Teardown() from the
    // base class.
};

TEST_F(SocDigitsTest, set_valid_state_of_charges)
{
    App_SocDigits_Tick(soc_digits, 0.0f);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(1, App_SocDigits_GetNumDigits(soc_digits));

    App_SocDigits_Tick(soc_digits, 50.0f);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(5, App_SocDigits_GetDigits(soc_digits)[1]);
    ASSERT_EQ(2, App_SocDigits_GetNumDigits(soc_digits));

    App_SocDigits_Tick(soc_digits, 100.0f);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[1]);
    ASSERT_EQ(1, App_SocDigits_GetDigits(soc_digits)[2]);
    ASSERT_EQ(3, App_SocDigits_GetNumDigits(soc_digits));
}

TEST_F(SocDigitsTest, set_valid_state_of_charges_with_trailing_decimals)
{
    App_SocDigits_Tick(soc_digits, 0.12345f);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(1, App_SocDigits_GetNumDigits(soc_digits));

    App_SocDigits_Tick(soc_digits, 50.12345f);
    ASSERT_EQ(0, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(5, App_SocDigits_GetDigits(soc_digits)[1]);
    ASSERT_EQ(2, App_SocDigits_GetNumDigits(soc_digits));

    App_SocDigits_Tick(soc_digits, 99.12345f);
    ASSERT_EQ(9, App_SocDigits_GetDigits(soc_digits)[0]);
    ASSERT_EQ(9, App_SocDigits_GetDigits(soc_digits)[1]);
    ASSERT_EQ(2, App_SocDigits_GetNumDigits(soc_digits));
}

TEST_F(SocDigitsTest, set_invalid_state_of_charges)
{
    ErrorCode error_code;

    error_code = App_SocDigits_Tick(
        soc_digits,
        0.0f - std::nextafter(0.0f, std::numeric_limits<float>::min()));
    ASSERT_EQ(error_code, ERROR_CODE_INVALID_ARGS);

    error_code = App_SocDigits_Tick(
        soc_digits,
        100.0f + std::nextafter(100.0f, std::numeric_limits<float>::max()));
    ASSERT_EQ(error_code, ERROR_CODE_INVALID_ARGS);
}
