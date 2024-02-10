#include "test_pdmBaseStateMachineTest.h"
#include "app_powerManager.h"
#include "io_efuse.h"

class EfuseProtocolTest : public PdmBaseStateMachineTest
{
};

TEST_F(EfuseProtocolTest, lvpwr_protocol_successful) {
    fake_io_efuse_getChannelCurrent_returnsForArgs(EFUSE_CHANNEL_LVPWR, 0.3);
}

TEST_F(EfuseProtocolTest, lvpwr_protocol_unseccessful) {
    
}
