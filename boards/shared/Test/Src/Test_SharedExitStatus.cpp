#include "Test_Shared.h"

extern "C"
{
#include "App_SharedExitStatus.h"
}

FAKE_VOID_FUNC(callback, struct ExitStatus);

class SharedExitStatusTest : public testing::Test
{
  protected:
    void SetUp() override { RESET_FAKE(callback); }

    ExitCode FuncThatReturnsExitCode(void)
    {
        return EXIT_CODE(EXIT_CODE_INVALID_ARGS);
    }

    ExitCode FuncThatReturnsExitCodeWithMessage(void)
    {
        return EXIT_MSG(EXIT_CODE_INVALID_ARGS, "bar");
    }
};

TEST_F(SharedExitStatusTest, exit_code_macro)
{
    // EXIT_CODE_OK should be ignored
    ExitStatus exit_status = App_SharedExitStatus_Get();
    EXIT_CODE(EXIT_CODE_OK);
    ExitStatus new_exit_status = App_SharedExitStatus_Get();
    ASSERT_TRUE(
        memcmp(&exit_status, &new_exit_status, sizeof(ExitStatus)) == 0);

    // non-EXIT_CODE_OK exit codes should update the exit status
    EXIT_CODE(EXIT_CODE_INVALID_ARGS);
    exit_status = App_SharedExitStatus_Get();
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_status.code);
    ASSERT_STREQ("TestBody", exit_status.caller);
    ASSERT_STREQ("", exit_status.message);

    // Indirect use case with a function
    FuncThatReturnsExitCode();
    exit_status = App_SharedExitStatus_Get();
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_status.code);
    ASSERT_STREQ("FuncThatReturnsExitCode", exit_status.caller);
    ASSERT_STREQ("", exit_status.message);
}

TEST_F(SharedExitStatusTest, exit_msg_macro)
{
    // EXIT_CODE_OK should be ignored
    ExitStatus exit_status = App_SharedExitStatus_Get();
    EXIT_MSG(EXIT_CODE_OK, "foo");
    ExitStatus new_exit_status = App_SharedExitStatus_Get();
    ASSERT_TRUE(
        memcmp(&exit_status, &new_exit_status, sizeof(ExitStatus)) == 0);

    // non-EXIT_CODE_OK exit codes should update the exit status
    EXIT_MSG(EXIT_CODE_INVALID_ARGS, "foo");
    exit_status = App_SharedExitStatus_Get();
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_status.code);
    ASSERT_STREQ("TestBody", exit_status.caller);
    ASSERT_STREQ("foo", exit_status.message);

    // Indirect use case with a function
    FuncThatReturnsExitCodeWithMessage();
    exit_status = App_SharedExitStatus_Get();
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_status.code);
    ASSERT_STREQ("FuncThatReturnsExitCodeWithMessage", exit_status.caller);
    ASSERT_STREQ("bar", exit_status.message);
}

TEST_F(SharedExitStatusTest, update_callback)
{
    App_SharedExitStatus_SetUpdateCallback(callback);

    // EXIT_CODE_OK should be ignored
    ExitStatus exit_status = App_SharedExitStatus_Get();
    EXIT_MSG(EXIT_CODE_OK, "foo");
    ExitStatus new_exit_status = App_SharedExitStatus_Get();
    ASSERT_TRUE(
        memcmp(&exit_status, &new_exit_status, sizeof(ExitStatus)) == 0);
    ASSERT_EQ(0, callback_fake.call_count);

    // non-EXIT_CODE_OK exit codes should update the exit status
    EXIT_MSG(EXIT_CODE_INVALID_ARGS, "foo");
    exit_status = App_SharedExitStatus_Get();
    ASSERT_EQ(EXIT_CODE_INVALID_ARGS, exit_status.code);
    ASSERT_STREQ("TestBody", exit_status.caller);
    ASSERT_STREQ("foo", exit_status.message);
    ASSERT_EQ(1, callback_fake.call_count);
}
