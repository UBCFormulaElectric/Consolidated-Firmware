#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{

ExitCode clearStatReg()
{
    RETURN_IF_ERR(sendCmd(CLRFLAG));
    RETURN_IF_ERR(sendCmd(CLOVUV));
    return ExitCode::EXIT_CODE_OK;
}

void getStatusReg() {}

} // namespace io::adbms