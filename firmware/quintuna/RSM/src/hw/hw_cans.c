#include "hw_can.h"
#include "main.h"

const CanHandle can2 = {
    .hcan    = &hcan2,
    .bus_num = 0,
    .ready   = false,
};