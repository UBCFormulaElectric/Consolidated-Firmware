#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
std::expected<void, ErrorCode> wakeup()
{
    return sendCmd(RDCFGA);
}
} // namespace io::adbms
