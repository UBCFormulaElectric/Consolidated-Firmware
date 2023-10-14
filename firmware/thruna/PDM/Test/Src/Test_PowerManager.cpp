#include "Test_Pdm.h"

extern "C"
{
#include "App_PowerManager.h"
}

FAKE_VOID_FUNC(Io_Efuse_SetChannel, EfuseChannel, bool);
FAKE_VALUE_FUNC(bool, Io_Efuse_IsChannelEnabled, EfuseChannel);

class PowerManagerTests : public ::testing::Test
{
  protected:
    void SetUp() override { RESET_FAKE(Io_Efuse_SetChannel); }
};

TEST_F(PowerManagerTests, App_PowerManager_Init)
{
    App_PowerManager_Init();
    Io_Efuse_IsChannelEnabled_fake.return_val = true;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        ASSERT_TRUE(Io_Efuse_IsChannelEnabled(static_cast<EfuseChannel>(efuse)));
    }
}

TEST_F(PowerManagerTests, App_PowerManager_SetState_Shutdown)
{
    App_PowerManager_SetState(POWER_MANAGER_CONTRACTOR_SHUTDOWN);
    Io_Efuse_IsChannelEnabled_fake.return_val = false;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        GTEST_ASSERT_FALSE(Io_Efuse_IsChannelEnabled(static_cast<EfuseChannel>(efuse)));
    }
}

TEST_F(PowerManagerTests, App_PowerManager_SetState_Drive)
{
    App_PowerManager_SetState(POWER_MANAGER_CONTRACTOR_DRIVE);
    Io_Efuse_IsChannelEnabled_fake.return_val = true;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        GTEST_ASSERT_TRUE(Io_Efuse_IsChannelEnabled(static_cast<EfuseChannel>(efuse)));
    }
}
