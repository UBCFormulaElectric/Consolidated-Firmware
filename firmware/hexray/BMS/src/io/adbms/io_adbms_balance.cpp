#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

namespace io::adbms
{
std::expected<void, ErrorCode> sendBalanceCmd()
{
    return sendCmd(UNMUTE);
}

std::expected<void, ErrorCode> sendStopBalanceCmd()
{
    return sendCmd(MUTE);
}
} // namespace io::adbms
