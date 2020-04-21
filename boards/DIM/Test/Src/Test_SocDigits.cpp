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
    App_SocDigits_Convert(0.0f, &soc_digits);
    ASSERT_EQ(0, soc_digits.digits[0]);
    ASSERT_EQ(1, soc_digits.num_digits);

    App_SocDigits_Convert(50.0f, &soc_digits);
    ASSERT_EQ(0, soc_digits.digits[0]);
    ASSERT_EQ(5, soc_digits.digits[1]);
    ASSERT_EQ(2, soc_digits.num_digits);

    App_SocDigits_Convert(100.0f, &soc_digits);
    ASSERT_EQ(0, soc_digits.digits[0]);
    ASSERT_EQ(0, soc_digits.digits[1]);
    ASSERT_EQ(1, soc_digits.digits[2]);
    ASSERT_EQ(3, soc_digits.num_digits);
}

TEST_F(SocDigitsTest, set_valid_state_of_charges_with_trailing_decimals)
{
    App_SocDigits_Convert(0.12345f, &soc_digits);
    ASSERT_EQ(0, soc_digits.digits[0]);
    ASSERT_EQ(1, soc_digits.num_digits);

    App_SocDigits_Convert(50.12345f, &soc_digits);
    ASSERT_EQ(0, soc_digits.digits[0]);
    ASSERT_EQ(5, soc_digits.digits[1]);
    ASSERT_EQ(2, soc_digits.num_digits);

    App_SocDigits_Convert(99.12345f, &soc_digits);
    ASSERT_EQ(9, soc_digits.digits[0]);
    ASSERT_EQ(9, soc_digits.digits[1]);
    ASSERT_EQ(2, soc_digits.num_digits);
}

TEST_F(SocDigitsTest, set_invalid_state_of_charges)
{
    ErrorCode error_code;
    float     epsilon;

    epsilon    = std::nextafter(0.0f, std::numeric_limits<float>::min());
    error_code = App_SocDigits_Convert(0.0f - epsilon, &soc_digits);
    ASSERT_EQ(error_code, ERROR_CODE_INVALID_ARGS);

    epsilon    = std::nextafter(100.0f, std::numeric_limits<float>::max());
    error_code = App_SocDigits_Convert(100.0f + epsilon, &soc_digits);
    ASSERT_EQ(error_code, ERROR_CODE_INVALID_ARGS);
}
