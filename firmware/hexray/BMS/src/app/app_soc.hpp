#pragma once

#include "app_pack.hpp"
#include "io_adbms.hpp"

#include <cstdint>

namespace app::soc
{
struct SocStats {
    uint32_t                  updated_ms = 0U;
    app::pack::LocatedValue   max{};
    app::pack::LocatedValue   min{};
    io::adbms::Cells<float>   soc{};
    app::pack::CellFlags      valid{};
};

void init();
void update();
void broadcast();
} // namespace app::soc
