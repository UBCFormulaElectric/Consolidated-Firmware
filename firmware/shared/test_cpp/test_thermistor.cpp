#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "app_thermistor.hpp"


// Simple monotonic decreasing LUT for hand-calculated expectations
static constexpr float simple_resistances[] = {100.0f, 80.0f, 60.0f, 40.0f, 20.0f};

// Constexpr / compile-time checks
static_assert(app::therm::ThermistorLUT(simple_resistances, 0.0f, 1.0f).size() == 5U, "ThermistorLUT size mismatch at compile time");

TEST(ThermistorLUTTest, compile_time_constructor_and_accessors)
{
    constexpr float starting = 0.0f;
    constexpr float resolution = 1.0f;
    constexpr app::therm::ThermistorLUT lut( simple_resistances, starting, resolution);

    ASSERT_EQ(lut.size(), 5u);
    ASSERT_FLOAT_EQ(lut.starting_temp(), starting);
    ASSERT_FLOAT_EQ(lut.resolution(), resolution);
    ASSERT_EQ(lut.resistances(), simple_resistances);
}

TEST(ThermistorLUTTest, exact_match)
{
    app::therm::ThermistorLUT lut(simple_resistances, 0.0f, 1.0f);

    // Temperature = starting_temp + index * resolution
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(100.0f), 0.0f); // idx 0
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(80.0f), 1.0f);  // idx 1
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(60.0f), 2.0f);  // idx 2
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(40.0f), 3.0f);  // idx 3
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(20.0f), 4.0f);  // idx 4
}

TEST(ThermistorLUTTest, basic_interpolation_value_checks)
{
    app::therm::ThermistorLUT lut(simple_resistances, 0.0f, 1.0f);

    ASSERT_NEAR(lut.resistanceToTemp(90.0f), 0.5f, 1e-3f);
    ASSERT_NEAR(lut.resistanceToTemp(70.0f), 1.5f, 1e-3f);
    ASSERT_NEAR(lut.resistanceToTemp(50.0f), 2.5f, 1e-3f);
    ASSERT_NEAR(lut.resistanceToTemp(30.0f), 3.5f, 1e-3f);
    
    // Test a 25% alignment. 85 is 75% between 100 (idx 0) and 80 (idx 1).
    // 85 is 15 away from 100, meaning 15/20 (0.75) along the temperature change.
    // Temperature for 100 is 0, for 80 is 1. (85-80)*((0-1)/(100-80)) + 1 = 5*(-1/20) + 1 = -0.25 + 1 = 0.75.
    ASSERT_NEAR(lut.resistanceToTemp(85.0f), 0.75f, 1e-3f);
}

//* Check with Kevin if overlapping with header file tests
TEST(ThermistorLUTTest, out_of_range)
{
    app::therm::ThermistorLUT lut(simple_resistances, 0.0f, 1.0f);

    // Out of bounds (higher than first, lower than last) should return -1.0f
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(100.1f), -1.0f); 
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(19.9f), -1.0f);
}

//* Check with Kevin if overlapping with header file tests
TEST(ThermistorLUTTest, single_entry)
{
    constexpr float single[] = {100.0f};
    app::therm::ThermistorLUT lut(single, 5.0f, 1.0f);

    ASSERT_FLOAT_EQ(lut.resistanceToTemp(100.0f), 5.0f);
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(100.1f), -1.0f);
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(99.9f), -1.0f);
}

//* Check with Kevin if overlapping with header file tests
TEST(ThermistorLUTTest, nan_and_inf)
{
    app::therm::ThermistorLUT lut(simple_resistances, 0.0f, 1.0f);

    ASSERT_FLOAT_EQ(lut.resistanceToTemp(std::numeric_limits<float>::quiet_NaN()), -1.0f);
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(std::numeric_limits<float>::infinity()), -1.0f);
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(-std::numeric_limits<float>::infinity()), -1.0f);
}

TEST(ThermistorLUTTest, degenerate_adjacent_equal)
{
    constexpr float degenerate[] = {100.0f, 80.0f, 80.0f, 60.0f};
    app::therm::ThermistorLUT lut(degenerate, 0.0f, 1.0f);

    // According to the code, if y2 == y1, it returns x1 (the temp of the higher index)
    // 80 maps to idx 1 and idx 2. Temp for idx 1 = 1.0, idx 2 = 2.0. Should return 2.0 or 1.0 based on binary search.
    // It will return temp based on where the search settles. Let's make sure it doesn't divide by zero.
    float result = lut.resistanceToTemp(80.0f);
    ASSERT_TRUE(std::isfinite(result));
}

TEST(ThermistorLUTTest, binary_search_edge_cases)
{
    constexpr float binary_res[] = {100.0f, 90.0f, 80.0f, 70.0f, 60.0f, 50.0f, 40.0f, 30.0f, 20.0f};
    app::therm::ThermistorLUT lut(binary_res, 0.0f, 1.0f);

    // Pick points exactly between array entries specifically to trace through low/mid/high index shifts thoroughly
    ASSERT_NEAR(lut.resistanceToTemp(95.0f), 0.5f, 1e-3f);
    ASSERT_NEAR(lut.resistanceToTemp(75.0f), 2.5f, 1e-3f);
    ASSERT_NEAR(lut.resistanceToTemp(25.0f), 7.5f, 1e-3f);
}

TEST(ThermistorLUTTest, precondition_decreasing_resistances)
{
    // The class assumes LUT resistances are strictly monotonically decreasing, i.e., resistances[0] >= ... >= resistances[size-1]
    // Since there isn't a runtime throw built-in for this, we add a unit test demonstrating the behavior is currently unbounded or undefined,
    // thereby explicitly documenting it as a precondition test (to warn future developers).
    constexpr float non_monotonic[] = {100.0f, 60.0f, 80.0f, 20.0f};
    app::therm::ThermistorLUT lut(non_monotonic, 0.0f, 1.0f);

    // For a non-monotonic list, checking value 70 might yield invalid results compared to a sensible expectation
    float t = lut.resistanceToTemp(70.0f);
    ASSERT_GE(t, 0.0f) << "Violation of monotonicity leads to potentially confusing output just mathematically applied.";
}
