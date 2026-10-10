#include <iostream>

#include "app_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_ntpButton.hpp"
#include "io_telemRx.hpp"
#include <expected>
#include <util_errorCodes.hpp>
io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_fileSystems.hpp"
#include <algorithm>
io::FileSystem fs{};

// io::FileSystem methods are faked in shared/srcpp_fake/io_filesystem.cpp

bool io::ntpButton::isPressed()
{
    return false;
}

std::expected<std::span<const uint8_t>, ErrorCode> io::telemRx::read(std::span<uint8_t>)
{
    return std::span<const uint8_t>{};
}

#include "io_shdn_loop.hpp"
const io::shdn::node r_estop(app::can_tx::DAM_REStopOKStatus_set);
const io::shdn::node l_estop(app::can_tx::DAM_LEStopOKStatus_set);
