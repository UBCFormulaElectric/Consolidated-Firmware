#include "Test_Shared.h"

extern "C"
{
#include "App_SharedWaitSignal.h"
}

FAKE_VALUE_FUNC(bool, wait_is_high, struct World *);
FAKE_VOID_FUNC(wait_callback_function, struct World *);

class SharedWaitSignalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        callback.function         = wait_callback_function;
        callback.wait_duration_ms = 0;
        wait_signal = App_SharedWaitSignal_Create(0, wait_is_high, world, callback);

        RESET_FAKE(wait_is_high);
        RESET_FAKE(wait_callback_function);
    }

    void TearDown() override
    {
        TearDownObject(wait_signal, App_SharedWaitSignal_Destroy);
    }

    struct WaitSignal *       wait_signal;
    struct WaitSignalCallback callback;
    struct TestWorld *        world;
};
