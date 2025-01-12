#include "io_shdnLoopNode.hpp"

namespace io::shdn
{
bool node::is_ok() const
{
    return pin.readPin();
}
void node::broadcast() const
{
    can_broadcast(is_ok());
}
} // namespace io::shdn
