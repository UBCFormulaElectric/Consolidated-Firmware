#pragma once

#include "app_pack.hpp"
#include "io_adbms.hpp"

#include <cstdint>
#include <util_errorCodes.hpp>

namespace app::soc
{
struct SocBuffer
{
    uint32_t                    updated_ms = 0U;
    app::pack::Reading          max{};
    app::pack::Reading          min{};
    io::adbms::PackCells<std::expected<float, ErrorCode>> soc{};
};

void init();
void update();
void broadcast();
} // namespace app::soc