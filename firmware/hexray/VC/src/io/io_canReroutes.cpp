#include "hw_cans.hpp"
#include "io_canReroute.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "io_canMsg.hpp"
namespace io::canReroute
{
void transmitFDCAN(const JsonCanMsg &msg)
{
    hw::can::fdcan1.can_transmit(msg);
}

void transmitInvCAN(const JsonCanMsg &msg)
{
    hw::can::invcan.can_transmit(msg);
}

void CANReroute_init()
{
    io::can_reroute::init(transmitFDCAN, transmitInvCAN);
}

void handleCallback(const io::CanMsg &rx_msg)
{
    JsonCanMsg json_can_msg = app::jsoncan::copyFromCanMsg(rx_msg);
    
    io::can_reroute::reroute_FDCAN(json_can_msg);
    io::can_reroute::reroute_InvCAN(json_can_msg);
}
}