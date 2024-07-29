#include "io_switch.h"

namespace io
{
bool Switch::isClosed() const
{
    return pin.readPin();
}
} // namespace io