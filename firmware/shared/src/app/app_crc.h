#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t app_crc_calculate(const void *buf, size_t size);
