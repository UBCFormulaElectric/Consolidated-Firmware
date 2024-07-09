#include "io_jsoncan.h"
#include "hw_can.h"
#include <cstring>

void io_jsoncan_copyToCanMsg(const JsonCanMsg *src, hw::CanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}

void io_jsoncan_copyFromCanMsg(const hw::CanMsg *src, JsonCanMsg *dest)
{
    dest->std_id = src->std_id;
    dest->dlc    = src->dlc;
    memcpy(dest->data, src->data, sizeof(dest->data));
}
