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

    // Use ASSERT_NEAR for interpolation results to handle standard floating point drift
    ASSERT_NEAR(lut.resistanceToTemp(100.0f), 0.0f, 1e-5f); // idx 0
    ASSERT_NEAR(lut.resistanceToTemp(80.0f), 1.0f, 1e-5f);  // idx 1
    ASSERT_NEAR(lut.resistanceToTemp(60.0f), 2.0f, 1e-5f);  // idx 2
    ASSERT_NEAR(lut.resistanceToTemp(40.0f), 3.0f, 1e-5f);  // idx 3
    ASSERT_NEAR(lut.resistanceToTemp(20.0f), 4.0f, 1e-5f);  // idx 4
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

//Uses other constructor
TEST(ThermistorLUTTest, null_pointer_and_empty_test) {
     // Test explicit pointer constructor with nullptr and size 0
    app::therm::ThermistorLUT null_lut(nullptr, 0.0f, 1.0f, 0U);
    ASSERT_FLOAT_EQ(null_lut.resistanceToTemp(50.0f), -1.0f);

    // Test a valid pointer but manually given size 0
    app::therm::ThermistorLUT empty_lut(simple_resistances, 0.0f, 1.0f, 0U);
    ASSERT_FLOAT_EQ(empty_lut.resistanceToTemp(50.0f), -1.0f);
}

//tested because a size 2 lut skips while (high_index > low_index + 1U)
TEST(ThermistorLUTTest, two_entry_lut)
{
    constexpr float two_entries[] = {100.0f, 50.0f};
    app::therm::ThermistorLUT lut(two_entries, 10.0f, 2.0f); // 100 -> 10C, 50 -> 12C

    // Exactly halfway: 75 ohms should be 11C
    ASSERT_NEAR(lut.resistanceToTemp(75.0f), 11.0f, 1e-4f);
    
    // Bounds limits
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(100.1f), -1.0f);
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(49.9f), -1.0f);
}

TEST(ThermistorLUTTest, four_input_constructor)
{
    // Pass strictly as a pointer and specific size to trigger the non-template constructor
    const float* ptr = simple_resistances;
    app::therm::ThermistorLUT lut(ptr, 5.0f, 0.5f, 3U); // Artificially restricting size to 3

    ASSERT_EQ(lut.size(), 3U);
    
    // Should pass since 60 is within the first 3 elements (100, 80, 60)
    ASSERT_NEAR(lut.resistanceToTemp(60.0f), 6.0f, 1e-4f); 
    
    // 40 is valid in the array, but out of bounds based on our restricted size parameter
    ASSERT_FLOAT_EQ(lut.resistanceToTemp(40.0f), -1.0f); 
}

TEST(ThermistorLUTTest, production_array_integration)
{
    // Exact production array from app_thermistors.cpp
    constexpr float adbms_ntc10k_lut_buffer[] = {
        29239.8f, 28571.6f, 27920.7f, 27286.6f, 26668.8f, 26066.9f, 25480.3f, 24908.8f, 24351.8f, 23808.9f, 23279.8f,
        22764.0f, 22261.2f, 21771.0f, 21293.0f, 20827.0f, 20372.5f, 19929.4f, 19497.2f, 19075.6f, 18664.4f, 18263.3f,
        17872.0f, 17490.3f, 17117.8f, 16754.3f, 16399.7f, 16053.6f, 15715.8f, 15386.1f, 15064.3f, 14750.1f, 14443.4f,
        14144.0f, 13851.7f, 13566.3f, 13287.5f, 13015.3f, 12749.5f, 12489.9f, 12236.3f, 11988.6f, 11746.6f, 11510.2f,
        11279.2f, 11053.5f, 10833.0f, 10617.5f, 10407.0f, 10201.2f, 10000.0f, 9803.4f,  9611.2f,  9423.3f,  9239.7f,
        9060.1f,  8884.5f,  8712.9f,  8545.0f,  8380.8f,  8220.3f,  8063.3f,  7909.7f,  7759.4f,  7612.5f,  7468.7f,
        7328.1f,  7190.5f,  7055.8f,  6924.1f,  6795.2f,  6669.0f,  6545.5f,  6424.7f,  6306.4f,  6190.7f,  6077.3f,
        5966.4f,  5857.8f,  5751.4f,  5647.3f,  5545.4f,  5445.5f,  5347.8f,  5252.1f,  5158.3f,  5066.5f,  4976.5f,
        4888.4f,  4802.1f,  4717.5f,  4634.7f,  4553.5f,  4474.0f,  4396.1f,  4319.8f,  4244.9f,  4171.6f,  4099.8f,
        4029.3f,  3960.3f,  3892.6f,  3826.3f,  3761.3f,  3697.5f,  3635.0f,  3573.8f,  3513.7f,  3454.8f,  3397.0f,
        3340.4f,  3284.8f,  3230.3f,  3176.9f,  3124.5f,  3073.1f,  3022.7f,  2973.2f,  2924.7f,  2877.0f,  2830.3f,
        2784.5f,  2739.5f,  2695.4f,  2652.1f,  2609.6f,  2567.9f,  2526.9f,  2486.8f,  2447.4f,  2408.6f,  2370.6f,
        2333.4f,  2296.7f,  2260.8f,  2225.5f,  2190.9f,  2156.8f,  2123.4f,  2090.6f,  2058.4f,  2026.8f,  1995.7f,
        1965.2f,  1935.2f,  1905.8f,  1876.9f,  1848.5f,  1820.6f,  1793.2f,  1766.2f,  1739.8f,  1713.8f,  1688.2f,
        1663.1f,  1638.5f,  1614.2f,  1590.4f,  1567.0f,  1544.0f,  1521.4f,  1499.2f,  1477.3f,  1455.9f,  1434.8f,
        1414.0f,  1393.6f,  1373.6f,  1353.9f,  1334.5f,  1315.4f,  1296.7f,  1278.3f,  1260.2f,  1242.3f,  1224.8f,
        1207.6f,  1190.6f,  1174.0f,  1157.6f,  1141.4f,  1125.6f,  1110.0f,  1094.6f,  1079.5f,  1064.7f,  1050.1f,
        1035.7f,  1021.5f,  1007.6f,  993.9f,   980.4f,   967.2f,   954.1f,   941.3f,   928.6f,   916.2f,   903.9f,
        891.9f,   880.0f,   868.4f 
    };

    // Starting temp = 0.0f, Resolution = 0.5f
    app::therm::ThermistorLUT lut(adbms_ntc10k_lut_buffer, 0.0f, 0.5f);
    
    ASSERT_EQ(lut.size(), 201U);

    // Exact matches
    ASSERT_NEAR(lut.resistanceToTemp(29239.8f), 0.0f, 1e-4f);  // index 0 -> 0.0 C
    ASSERT_NEAR(lut.resistanceToTemp(10000.0f), 25.0f, 1e-4f); // index 50 -> 25.0 C
    ASSERT_NEAR(lut.resistanceToTemp(868.4f), 100.0f, 1e-4f);  // index 200 -> 100.0 C

    // Interpolation test exactly halfway between 25.0 C (10000.0) and 25.5 C (9803.4)
    // Exactly halfway resistance is 9901.7, expect 25.25 C
    ASSERT_NEAR(lut.resistanceToTemp(9901.7f), 25.25f, 1e-4f);
}