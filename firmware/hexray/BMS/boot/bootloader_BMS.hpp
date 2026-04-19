#pragma once

#include <cstdint>

extern "C"
{
#include "app_commitInfo.h"
}

constexpr uint32_t board_highbits       = 0x1C000000;
constexpr uint32_t git_commit_has_val   = GIT_COMMIT_HASH;
constexpr bool     git_commit_clean_val = GIT_COMMIT_CLEAN;
