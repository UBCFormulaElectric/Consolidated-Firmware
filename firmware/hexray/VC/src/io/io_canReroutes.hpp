#pragma once

#include "hw_can.hpp"
#include "io_canReroute.hpp"

namespace io::canReroute
{
void handleCallback(const hw::CanMsg &hw_rx_msg);
}