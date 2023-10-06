#include <gtest/gtest.h>
#include "fake_test_header.hpp"

TEST(test_fakegen, test_void_func_no_params)
{
    // Test call count works.
    ASSERT_EQ(fake_voidFuncNoParams_callCount(), 0);
    voidFuncNoParams();
    ASSERT_EQ(fake_voidFuncNoParams_callCount(), 1);
    voidFuncNoParams();
    ASSERT_EQ(fake_voidFuncNoParams_callCount(), 2);

    // Test reset resets call count.
    fake_voidFuncNoParams_reset();
    ASSERT_EQ(fake_voidFuncNoParams_callCount(), 0);
}

TEST(test_fakegen, test_value_returning_func_no_params)
{
    // Test initial conditions.
    ASSERT_EQ(fake_valueReturningFuncNoParams_callCount(), 0);
    ASSERT_EQ(valueReturningFuncNoParams(), 0);
    ASSERT_EQ(fake_valueReturningFuncNoParams_callCount(), 1);

    // Test changing the return value.
    fake_valueReturningFuncNoParams_returns(10);
    ASSERT_EQ(valueReturningFuncNoParams(), 10);
    ASSERT_EQ(fake_valueReturningFuncNoParams_callCount(), 2);
    fake_valueReturningFuncNoParams_returns(20);
    ASSERT_EQ(valueReturningFuncNoParams(), 20);
    ASSERT_EQ(fake_valueReturningFuncNoParams_callCount(), 3);

    // Test reset resets return value and call count.
    fake_valueReturningFuncNoParams_reset();
    ASSERT_EQ(fake_valueReturningFuncNoParams_callCount(), 0);
    ASSERT_EQ(valueReturningFuncNoParams(), 0);
}

TEST(test_fakegen, test_void_func_params)
{
    // Test initial conditions.
    ASSERT_EQ(fake_voidFuncParams_callCount(), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 0, 0), 0);

    // Test call counts by invocation.
    voidFuncParams(0, 1, 2);
    ASSERT_EQ(fake_voidFuncParams_callCount(), 1);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 1, 2), 1);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(1, 1, 2), 0);

    voidFuncParams(0, 1, 2);
    voidFuncParams(0, 1, 2);
    voidFuncParams(3, 4, 5);
    voidFuncParams(6, 7, 8);
    ASSERT_EQ(fake_voidFuncParams_callCount(), 5);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 1, 2), 3);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(3, 4, 5), 1);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(6, 7, 8), 1);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 1, 1), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(4, 4, 5), 0);

    // Test reset resets call count.
    fake_voidFuncParams_reset();
    ASSERT_EQ(fake_voidFuncParams_callCount(), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 1, 2), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(3, 4, 5), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(6, 7, 8), 0);
    ASSERT_EQ(fake_voidFuncParams_callCountForArgs(0, 0, 0), 0);
}

TEST(test_fakegen, test_value_returning_func_params)
{
    // Test initial conditions.
    ASSERT_EQ(fake_valueReturningFuncParams_callCount(), 0);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 0), 0);

    ASSERT_EQ(valueReturningFuncParams(0, 0), 0);
    ASSERT_EQ(fake_valueReturningFuncParams_callCount(), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 0), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 1), 0);

    // Test setting return value by invocation.
    fake_valueReturningFuncParams_returnsForArgs(10, 20, 30);
    fake_valueReturningFuncParams_returnsForArgs(-1, 3.14f, 24);

    ASSERT_EQ(valueReturningFuncParams(10, 20), 30);
    ASSERT_EQ(valueReturningFuncParams(10, 10), 0);
    ASSERT_EQ(valueReturningFuncParams(-1, 3.14f), 24);
    ASSERT_EQ(valueReturningFuncParams(-1, 3.15f), 0);

    // Test call counts by invocation.
    valueReturningFuncParams(10, 20);
    ASSERT_EQ(fake_valueReturningFuncParams_callCount(), 6);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(10, 20), 2);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(-1, 3.14f), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(10, 10), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(-1, 3.15f), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 0), 1);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 1), 0);

    // Test reset resets return value and call count.
    fake_valueReturningFuncParams_reset();
    ASSERT_EQ(fake_valueReturningFuncParams_callCount(), 0);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(10, 20), 0);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(-1, 3.14f), 0);
    ASSERT_EQ(fake_valueReturningFuncParams_callCountForArgs(0, 0), 0);

    ASSERT_EQ(valueReturningFuncParams(10, 20), 0);
    ASSERT_EQ(valueReturningFuncParams(10, 10), 0);
    ASSERT_EQ(valueReturningFuncParams(-1, 3.14f), 0);
}
