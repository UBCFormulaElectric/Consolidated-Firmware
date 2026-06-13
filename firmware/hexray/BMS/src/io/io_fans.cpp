#include "io_fans.hpp"
#include "hw_gpios.hpp"
#include <cstdint>

constexpr uint8_t MAX_RETRIES = 5;
namespace io::fans
{

void tick(const bool enable)
{
    fan_en.writePin(enable);
    // if (enable)
    // {
    //
    //     if (!fan_pgood.readPin())
    //     {
    //         static uint8_t num_retries = 0;
    //         if (num_retries < MAX_RETRIES)
    //         {
    //             num_retries++;
    //             fan_en.writePin(false);
    //         }
    //     }
    // }
    // else
    // {
    //     fan_en.writePin(false);
    // }
}
} // namespace io::fans
