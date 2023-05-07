#include "Test_Pdm.h"

extern "C" {
#include "App_CanUtils.h"
}

// PDM-2
TEST(CanMsgsTest, heartbeat_message_frequency) {
  ASSERT_GE(HZ_TO_MS(1), CAN_MSG_PDM_VITALS_CYCLE_TIME_MS);
}
