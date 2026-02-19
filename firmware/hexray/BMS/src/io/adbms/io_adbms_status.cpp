#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{

std::expected<void, ErrorCode> clearStatReg()
{
    RETURN_IF_ERR(sendCmd(CLRFLAG));
    RETURN_IF_ERR(sendCmd(CLOVUV));
    return {};
}

} // namespace io::adbms