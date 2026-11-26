#include "io_sd.hpp"

// Host test build: the SD bus/speed upgrade is a no-op since there's no STM32 SDMMC peripheral.
namespace io::sd
{
result<void> upgrade()
{
    return {};
}
} // namespace io::sd
