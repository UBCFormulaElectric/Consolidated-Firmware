#pragma once

#include <cstdint>
#include <span>

#include "util_errorCodes.hpp"

namespace io::telemUart
{

#ifdef TARGET_EMBEDDED
[[nodiscard]] result<void> tx_data);
#else
[[nodiscard]] inline result<void> tx_data)
{
    (void)tx_data;
    return {};
}
#endif

} // namespace io::telemUart
