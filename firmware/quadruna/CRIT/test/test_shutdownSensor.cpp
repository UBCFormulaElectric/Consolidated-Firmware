#include <gtest/gtest.h>
#include "fake_io_shutdownSensor.hpp"

extern "C"
{
#include "io_shutdownSensor.h"
}

class ShutdownSensorTest : public testing::Test
{
  protected:
    ShutdownSensor sensor;

    void SetUp() override { fake_io_shutdownSensor_readPin_reset(); }
};

TEST_F(ShutdownSensorTest, VerifyReturnValue)
{
    // Set the fake to return true and verify the return value.
    fake_io_shutdownSensor_readPin_returnsForAnyArgs(true);
    ASSERT_TRUE(io_shutdownSensor_readPin(&sensor));

    // Set the fake to return false and verify the return value.
    fake_io_shutdownSensor_readPin_reset(); // Reset to clear previous state.
    fake_io_shutdownSensor_readPin_returnsForAnyArgs(false);
    ASSERT_FALSE(io_shutdownSensor_readPin(&sensor));
}

TEST_F(ShutdownSensorTest, VerifyCallCount)
{
    // Call the function without setting a specific return value.
    io_shutdownSensor_readPin(&sensor);

    // Verify that the call count is as expected.
    ASSERT_EQ(1, fake_io_shutdownSensor_readPin_callCount());
}

TEST_F(ShutdownSensorTest, VerifyCallCountForSpecificArgs)
{
    // Set the function to return a specific value when called with a specific sensor.
    fake_io_shutdownSensor_readPin_returnsForArgs(&sensor, true);

    // Call the function with the sensor.
    io_shutdownSensor_readPin(&sensor);

    // Verify that the function was called with the specific arguments.
    ASSERT_EQ(1, fake_io_shutdownSensor_readPin_callCountForArgs(&sensor));
}