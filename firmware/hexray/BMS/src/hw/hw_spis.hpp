#pragma once

extern "C"
{
#include "main.h"
}
#include "hw_spi.hpp"
#include "hw_gpios.hpp"

namespace hw::spi
{
extern const SpiDevice adbms_spi_ls;
extern const SpiDevice adbms_spi_hs;
} // namespace hw::spi
