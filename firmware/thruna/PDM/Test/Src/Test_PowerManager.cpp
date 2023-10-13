#include "Test_Pdm.h"

extern "C"
{
#include "App_powerManager.h"
}

FAKE_VOID_FUNC(Io_Efuse_SetChannel, int, bool);

class PowerManagerTests : public ::testing::Test
{
  protected:
    void SetUp() override { RESET_FAKE(Io_Efuse_SetChannel); }
};

TEST_F(PowerManagerTests, PowerManager_Init)
{
    PowerManager_Init();
    
    ASSERT_EQ(Io_Efuse_IsChannelEnabled(EFUSE_CHANNEL_AIR), true);
    ASSERT_EQ(Io_Efuse_IsChannelEnabled(EFUSE_CHANNEL_LVPWR), true);
    ASSERT_EQ(Io_Efuse_IsChannelEnabled(EFUSE_CHANNEL_DI_LHS), true);
    ASSERT_EQ(Io_Efuse_IsChannelEnabled(EFUSE_CHANNEL_DI_RHS), true);
    ASSERT_EQ(Io_Efuse_IsChannelEnabled(EFUSE_CHANNEL_AUX), false);
}

TEST_F(PowerManagerTests, PowerManager_SetState_ContractorOpen)
{
    PowerManager_SetState(POWER_MANAGER_CONTRACTOR_OPEN);

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        ASSERT_EQ(Io_Efuse_IsChannelEnabled(static_cast<EfuseChannel>(efuse)), false);
    }
}

TEST_F(PowerManagerTests, PowerManager_SetState_ContractorClosed)
{
    PowerManager_SetState(POWER_MANAGER_CONTRACTOR_CLOSED);

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        ASSERT_EQ(Io_Efuse_IsChannelEnabled(static_cast<EfuseChannel>(efuse)), true);
    }
}
