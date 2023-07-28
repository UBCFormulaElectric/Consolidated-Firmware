#include "Test_Bms.h"

extern "C"
{
#include "App_OkStatus.h"
}

FAKE_VALUE_FUNC(ExitCode, enable_status_ok);
FAKE_VALUE_FUNC(ExitCode, disable_status_ok);
FAKE_VALUE_FUNC(bool, is_ok_status_enabled);

class OkStatusTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        ok_status = App_OkStatus_Create(enable_status_ok, disable_status_ok, is_ok_status_enabled);

        RESET_FAKE(enable_status_ok);
        RESET_FAKE(disable_status_ok);
        RESET_FAKE(is_ok_status_enabled);
    }

    void TearDown() override { TearDownObject(ok_status, App_OkStatus_Destroy); }

    struct OkStatus *ok_status;
};

TEST_F(OkStatusTest, enable_returns_ok)
{
    enable_status_ok_fake.return_val = EXIT_CODE_OK;
    ASSERT_EQ(EXIT_CODE_OK, App_OkStatus_Enable(ok_status));
    ASSERT_EQ(1, enable_status_ok_fake.call_count);
}

TEST_F(OkStatusTest, enable_returns_unimplemented)
{
    enable_status_ok_fake.return_val = EXIT_CODE_UNIMPLEMENTED;
    ASSERT_EQ(EXIT_CODE_UNIMPLEMENTED, App_OkStatus_Enable(ok_status));
    ASSERT_EQ(1, enable_status_ok_fake.call_count);
}

TEST_F(OkStatusTest, disable_returns_ok)
{
    disable_status_ok_fake.return_val = EXIT_CODE_OK;
    ASSERT_EQ(EXIT_CODE_OK, App_OkStatus_Disable(ok_status));
    ASSERT_EQ(1, disable_status_ok_fake.call_count);
}

TEST_F(OkStatusTest, disable_returns_unimplemented)
{
    disable_status_ok_fake.return_val = EXIT_CODE_UNIMPLEMENTED;
    ASSERT_EQ(EXIT_CODE_UNIMPLEMENTED, App_OkStatus_Disable(ok_status));
    ASSERT_EQ(1, disable_status_ok_fake.call_count);
}

TEST_F(OkStatusTest, is_enabled_returns_true)
{
    is_ok_status_enabled_fake.return_val = true;
    ASSERT_TRUE(App_OkStatus_IsEnabled(ok_status));
    ASSERT_EQ(1, is_ok_status_enabled_fake.call_count);
}

TEST_F(OkStatusTest, is_enabled_returns_false)
{
    is_ok_status_enabled_fake.return_val = false;
    ASSERT_FALSE(App_OkStatus_IsEnabled(ok_status));
    ASSERT_EQ(1, is_ok_status_enabled_fake.call_count);
}
