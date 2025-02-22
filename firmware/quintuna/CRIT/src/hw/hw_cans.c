#include "hw_cans.h"
#include "main.h"

CanHandle can1 = { .hcan = &hcan2, .bus_num = 0, .ready = false };
