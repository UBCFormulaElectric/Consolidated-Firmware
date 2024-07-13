#include "io_driveMode.h"
#include "hw_gpios.h"

namespace io::driveMode
{
uint16_t readPins()
{
    return (uint16_t)((uint16_t)(hw::gpio::n_drive_mode_0_pin.readPin()) +
                      ((uint16_t)(hw::gpio::n_drive_mode_1_pin.readPin()) << 1) +
                      ((uint16_t)(hw::gpio::n_drive_mode_2_pin.readPin()) << 2) +
                      ((uint16_t)(hw::gpio::n_drive_mode_3_pin.readPin()) << 3));
}
} // namespace io::driveMode