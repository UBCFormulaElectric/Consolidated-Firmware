#include "io_bspdTest.h"
#include "stdbool.h"

void app_bspdPrefaul_pollBspdPrefault()
{
    bool brake_pressed = io_bspdTest_isBrakePressureThresholdExceeded();
    if (brake_pressed)
    {
    }
}
