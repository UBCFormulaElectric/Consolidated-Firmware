#include "io_jsoncan.h"
#include "hw_can.h"
#include <cstring>

namespace io::jsoncan
{
void copyToCanMsg(const JsonCanMsg *src, hw::CanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}

void copyFromCanMsg(const hw::CanMsg *src, JsonCanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}
} // namespace io::jsoncan
