#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED

#include "hw_gpios.h"
typedef struct
{
    const Gpio *current_status_gpio;
    const Gpio *latch_status_gpio;
    const bool  read_only; // Certain fault latches can only be read from.
    const bool  invert;
} FaultLatch;

#else
EMPTY_STRUCT(FaultLatch);
#endif

// latches to operate on
extern const FaultLatch bms_ok_latch;
extern const FaultLatch imd_ok_latch;
extern const FaultLatch bspd_ok_latch;

/**
 * Write a fault.
 * @param latch Fault latch instance.
 * @param status whether or not to set a fault (true = OK, false = fault).
 */
void io_faultLatch_setCurrentStatus(const FaultLatch *latch, bool status);

/**
 * Return whether or not there is currently a fault.
 * @param latch Fault latch instance.
 * @returns True = there is currently NOT a fault.
 */
bool io_faultLatch_getCurrentStatus(const FaultLatch *latch);

/**
 * Return whether or not the fault latch is OK.
 * @param latch Fault latch instance.
 * @returns True = fault has NOT been latched.
 */
bool io_faultLatch_getLatchedStatus(const FaultLatch *latch);
