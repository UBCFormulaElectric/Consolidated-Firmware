#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_powerMonitoring.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCPowerMonitorTest : public VCBaseTest
{
};

// updating CAN table for channels
TEST_F(VCPowerMonitorTest, updating_test) {
    io::fakes::powerMonitoring::set_reading_voltage(1, 5.0f);
    io::fakes::powerMonitoring::set_reading_current(1, 2.0f);
    io::fakes::powerMonitoring::set_reading_power(1, 10.0f);
    letTimePass(1);
    updateConfig();
    PowerManagerConfig                          state_{};
    std::array<io::Efuse *, NUM_EFUSE_CHANNELS> efuses_{};
    std::array<uint8_t, NUM_EFUSE_CHANNELS>     retries_{};
    ASSERT_FLOAT_EQ(5.0f, app::can_tx::VC_Voltage_Channel1_get());
    ASSERT_FLOAT_EQ(2.0f, app::can_tx::VC_Current_Channel1_get());
    ASSERT_FLOAT_EQ(10.0f, app::can_tx::VC_Power_Channel1_get());
}
