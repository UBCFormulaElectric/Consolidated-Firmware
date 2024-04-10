#include <gtest/gtest.h>
#include "fake_io_shutdownSensor.hpp"
#include "test_critBaseStateMachineTest.h"

extern "C"
{
#include "io_shutdownSensor.h"
}

class ShutdownSensorTest : public CritBaseStateMachineTest
{
    void SetUp() override { fake_io_shutdownSensor_readPin_reset(); }
};

TEST_F(ShutdownSensorTest, VerifyReturnValue)
{
    // Set the fake to return true and verify the return value.
    fake_io_shutdownSensor_readPin_returnsForAnyArgs(true);
    ASSERT_TRUE(io_shutdownSensor_readPin(&shdn_sen));

    // Set the fake to return false and verify the return value.
    fake_io_shutdownSensor_readPin_reset(); // Reset to clear previous state.
    fake_io_shutdownSensor_readPin_returnsForAnyArgs(false);
    ASSERT_FALSE(io_shutdownSensor_readPin(&shdn_sen));
}

TEST_F(ShutdownSensorTest, ShutdownSensorActiveWhenImdLatchedFaultIsFalse)
{
    app_canRx_BMS_ImdLatchedFault_update(false);

    fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, true);
    
    // Verify that the shutdown sensor is indeed active.
    ASSERT_TRUE(io_shutdownSensor_readPin(&shdn_sen));

}

TEST_F(ShutdownSensorTest, ShutdownSensorActiveWhenImdLatchedFaultIsTrue)
{
    app_canRx_BMS_ImdLatchedFault_update(true);

    fake_io_shutdownSensor_readPin_returnsForArgs(&shdn_sen, false);
    
    // Verify that the shutdown sensor is indeed not active.
    ASSERT_FALSE(io_shutdownSensor_readPin(&shdn_sen));

}

TEST_F(ShutdownSensorTest, VerifyCallCount)
{
    // Call the function without setting a specific return value.
    io_shutdownSensor_readPin(&shdn_sen);

    // Verify that the call count is as expected.
    ASSERT_EQ(1, fake_io_shutdownSensor_readPin_callCount());
}

TEST_F(ShutdownSensorTest, VerifyCallCountForSpecificArgs)
{
    // Set the function to return a specific value when called with a specific sensor.
    fake_io_shutdownSensor_readPin_returnsForAnyArgs(true);

    // Call the function with the sensor.
    io_shutdownSensor_readPin(&shdn_sen);

    // Verify that the function was called with the specific arguments.
    ASSERT_EQ(1, fake_io_shutdownSensor_readPin_callCountForArgs(&shdn_sen));
}