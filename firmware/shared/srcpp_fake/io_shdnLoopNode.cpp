#include "io_shdnLoopNode.hpp"

namespace io::shdn
{
bool node::is_ok() const
{
    return status;
}
void node::broadcast() const
{
    can_broadcast(is_ok());
}
void node::set_status(bool _status)
{
    this->status = _status;
}
} // namespace io::shdn
