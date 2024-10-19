#include "app_heartbeatMonitor.h"
#include "io_critHeartbeatNodes.h"

namespace app::heartbeat
{
static const monitor<4> critHeartbeat{ app_canTx_CRIT_Heartbeat_set,
                                       { &io::heartbeat::bms, &io::heartbeat::vc, &io::heartbeat::rsm,
                                         &io::heartbeat::fsm } };
}