#include "io/switch.hpp"

namespace io
{
bool Switch::isClosed() const
{
    return pin.readPin();
}
} // namespace io