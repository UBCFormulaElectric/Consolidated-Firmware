#include "Test_Fsm.h"

extern "C"
{
#include "App_AcceleratorPedal.h"
}

FAKE_VALUE_FUNC(bool, is_encoder_alarm_active);

class AcceleratorPedalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        accelerator_pedal =
            App_AcceleratorPedal_Create(is_encoder_alarm_active);
        RESET_FAKE(is_encoder_alarm_active);
    }

    void TearDown() override
    {
        TearDownObject(accelerator_pedal, App_AcceleratorPedal_Destroy);
    }

    struct AcceleratorPedal *accelerator_pedal;
};

TEST_F(AcceleratorPedalTest, active_alarm)
{
    is_encoder_alarm_active_fake.return_val = true;
    ASSERT_TRUE(App_AcceleratorPedal_IsEncoderAlarmActive(accelerator_pedal));
}

TEST_F(AcceleratorPedalTest, inactive_alarm)
{
    is_encoder_alarm_active_fake.return_val = false;
    ASSERT_FALSE(App_AcceleratorPedal_IsEncoderAlarmActive(accelerator_pedal));
}
