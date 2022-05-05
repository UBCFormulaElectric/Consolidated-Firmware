#include "Test_Fsm.h"

extern "C"
{
#include "App_AcceleratorPedals.h"
}

FAKE_VALUE_FUNC(bool, is_primary_encoder_alarm_active);
FAKE_VOID_FUNC(reset_primary_encoder_counter);
FAKE_VALUE_FUNC(uint32_t, get_primary_encoder_counter);
FAKE_VALUE_FUNC(bool, is_secondary_encoder_alarm_active);
FAKE_VOID_FUNC(reset_secondary_encoder_counter);
FAKE_VALUE_FUNC(uint32_t, get_secondary_encoder_counter);

class AcceleratorPedalTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        // Initialize the following accelerator pedal pair with the
        // primary_encoder_fully_pressed_value and
        // secondary_encoder_fully_pressed_value both set to 0. This is because
        // the specific value of both encoder's fully pressed values are not
        // required for the following tests
        accelerator_pedal_pair = App_AcceleratorPedals_Create(
            is_primary_encoder_alarm_active, is_secondary_encoder_alarm_active, get_primary_encoder_counter,
            get_secondary_encoder_counter, reset_primary_encoder_counter, reset_secondary_encoder_counter);
        RESET_FAKE(is_primary_encoder_alarm_active);
        RESET_FAKE(reset_primary_encoder_counter);
        RESET_FAKE(get_primary_encoder_counter);
        RESET_FAKE(is_secondary_encoder_alarm_active);
        RESET_FAKE(reset_secondary_encoder_counter);
        RESET_FAKE(get_secondary_encoder_counter);
    }

    void TearDown() override { TearDownObject(accelerator_pedal_pair, App_AcceleratorPedals_Destroy); }

    struct AcceleratorPedals *accelerator_pedal_pair;
};

TEST_F(AcceleratorPedalTest, active_primary_alarm)
{
    is_primary_encoder_alarm_active_fake.return_val = true;
    ASSERT_TRUE(App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(accelerator_pedal_pair));
}

TEST_F(AcceleratorPedalTest, inactive_primary_alarm)
{
    is_primary_encoder_alarm_active_fake.return_val = false;
    ASSERT_FALSE(App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(accelerator_pedal_pair));
}

TEST_F(AcceleratorPedalTest, active_secondary_alarm)
{
    is_secondary_encoder_alarm_active_fake.return_val = true;
    ASSERT_TRUE(App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(accelerator_pedal_pair));
}

TEST_F(AcceleratorPedalTest, inactive_secondary_alarm)
{
    is_secondary_encoder_alarm_active_fake.return_val = false;
    ASSERT_FALSE(App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(accelerator_pedal_pair));
}
