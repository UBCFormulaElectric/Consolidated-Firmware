#include <gtest/gtest.h>
#include "fake_io_driveMode.hpp"
#include "test_critBaseStateMachineTest.h"
#include <stdint.h>

class DriveModeTest : public CritBaseStateMachineTest
{
  protected:
    void SetUp() override { fake_io_driveMode_readPins_reset(); }
};

TEST_F(DriveModeTest, ReadSingleDriveMode)
{
    // Test reading a single drive mode value
    uint16_t expectedValue = 0b0001;
    fake_io_driveMode_readPins_returns(expectedValue);

    uint16_t driveMode = io_driveMode_readPins();
    ASSERT_EQ(expectedValue, driveMode);
    ASSERT_EQ(1u, fake_io_driveMode_readPins_callCount());
}

TEST_F(DriveModeTest, ReadMultipleDriveModes)
{
    // Setting up the fake to return different values on consecutive calls

    uint16_t firstValue = 0b0010;
    fake_io_driveMode_readPins_returns(firstValue);
    ASSERT_EQ(firstValue, io_driveMode_readPins());

    uint16_t secondValue = 0b0100;
    fake_io_driveMode_readPins_returns(secondValue);
    ASSERT_EQ(secondValue, io_driveMode_readPins());

    // Each call to io_driveMode_readPins should match the expected value
    // Verifying call count to ensure our fake is being called correctly
    ASSERT_EQ(2u, fake_io_driveMode_readPins_callCount());
}

TEST_F(DriveModeTest, VerifyDriveModeConsistency)
{
    // Verifying that the drive mode value is consistent across multiple reads
    uint16_t consistentValue = 0b1010;
    fake_io_driveMode_readPins_returns(consistentValue);

    ASSERT_EQ(consistentValue, io_driveMode_readPins());
    ASSERT_EQ(consistentValue, io_driveMode_readPins());

    // The drive mode should be read consistently, and the fake should be called each time
    ASSERT_EQ(2u, fake_io_driveMode_readPins_callCount());
}

TEST_F(DriveModeTest, ReadsPinStateZeroCorrectly)
{
    // Setup the fake to return 0 (all pins low in a 4-bit system: 0000 binary)
    fake_io_driveMode_readPins_returns(0);

    uint16_t result = io_driveMode_readPins();

    ASSERT_EQ(result, 0);
}

TEST_F(DriveModeTest, ReadsPinStateFifteenCorrectly)
{
    // Setup the fake to return 15 (all pins high in a 4-bit system: 1111 binary)
    fake_io_driveMode_readPins_returns(15);

    auto result = io_driveMode_readPins();

    ASSERT_EQ(result, 15);
}