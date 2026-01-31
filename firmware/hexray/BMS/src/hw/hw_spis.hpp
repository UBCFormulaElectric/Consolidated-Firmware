#pragma once

extern "C"
{
#include "firmware/hexray/BMS/src/cubemx/Inc/main.h"
}
#include "hw_spi.h"
#include "hw_gpios.h"

extern const SpiDevice adbms_spi_ls;
extern const SpiDevice adbms_spi_hs;