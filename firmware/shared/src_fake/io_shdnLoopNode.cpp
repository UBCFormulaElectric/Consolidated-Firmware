#include "io_shdnLoopNodeFake.hpp"

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
void node::set_status(bool status)
{
    this->status = status;
}
} // namespace io::shdn
