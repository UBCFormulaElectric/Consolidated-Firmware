#include "app_jsoncan.hpp"
#include "hw_can.hpp"
#include <cstring>

namespace io::jsoncan
{
void copyToCanMsg(const JsonCanMsg *src, hw::can::CanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}

void copyFromCanMsg(const hw::can::CanMsg *src, JsonCanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}
} // namespace io::jsoncan
