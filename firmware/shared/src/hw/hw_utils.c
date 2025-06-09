#include "hw_utils.h"

ExitCode hw_utils_convertHalStatus(HAL_StatusTypeDef status)
{
    switch (status)
    {
        case HAL_OK:
            return EXIT_CODE_OK;
        case HAL_ERROR:
            return EXIT_CODE_ERROR;
        case HAL_BUSY:
            return EXIT_CODE_BUSY;
        case HAL_TIMEOUT:
            return EXIT_CODE_TIMEOUT;
        default:
            return EXIT_CODE_OK;
    }
}
