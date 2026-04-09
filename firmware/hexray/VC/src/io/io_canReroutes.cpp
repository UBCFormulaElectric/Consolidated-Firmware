#include "io_canReroute.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "io_canReroutes.hpp"
#include "app_canUtils.hpp"

namespace io::canReroute
{
void handleCallback(const hw::CanMsg &hw_rx_msg)
{
    io::CanMsg io_rx_msg{ hw_rx_msg.std_id, hw_rx_msg.dlc, hw_rx_msg.data, true,
                          static_cast<app::can_utils::BusEnum>(0) };

    JsonCanMsg json_can_msg = app::jsoncan::copyFromCanMsg(io_rx_msg);

    io::can_reroute::reroute_InvCAN(json_can_msg);
    io::can_reroute::reroute_FDCAN(json_can_msg);
}
} // namespace io::canReroute