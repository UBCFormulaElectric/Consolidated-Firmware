#pragma once
#include "hw_spi.hpp"

namespace hw::spi
{
extern const SpiDevice adbms_spi_ls;
extern const SpiDevice adbms_spi_hs;
} // namespace hw::spi

// Backward-compatible aliases for existing unqualified references.
using hw::spi::adbms_spi_hs;
using hw::spi::adbms_spi_ls;
