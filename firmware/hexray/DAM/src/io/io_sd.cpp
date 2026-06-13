#include "io_sd.hpp"

#include "hw_sds.hpp"

namespace io::sd
{
result<void> upgrade()
{
    // The card is initialised in 1-bit bus width, so it must be upgraded to 4-bit before the speed
    // bump. Propagate the underlying hw error so callers can log what actually failed.
    if (const auto err = sd1.upgrade_buswidth(); !err.has_value())
        return std::unexpected(err.error());
    if (const auto err = sd1.update_speed(); !err.has_value())
        return std::unexpected(err.error());
    return {};
}
} // namespace io::sd
