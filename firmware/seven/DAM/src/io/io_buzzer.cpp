#include "io_buzzer.hpp"
#include "hw_gpios.hpp"

namespace io::buzzer
{
void enable()
{
    buzzer_pwr_en_pin.writePin(true);
}

void disable()
{
    buzzer_pwr_en_pin.writePin(false);
}
} // namespace io::buzzer
