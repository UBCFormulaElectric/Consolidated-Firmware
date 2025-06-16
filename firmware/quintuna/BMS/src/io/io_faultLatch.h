#pragma once

// I have made this compatible with booleans, don't make me regret it
typedef enum
{
    FAULT_LATCH_FAULT,
    FAULT_LATCH_OK,
} FaultLatchState;

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
typedef struct
{
    const Gpio *current_status_gpio;
    const Gpio *latch_status_gpio;
    const bool  read_only; // Certain fault latches can only be read from.
    const bool  invert;
} FaultLatch;

extern const FaultLatch bms_ok_latch;
extern const FaultLatch imd_ok_latch;
extern const FaultLatch bspd_ok_latch;
#else
typedef struct
{
    FaultLatchState status;
    FaultLatchState latched_state;
    const bool      read_only; // Certain fault latches can only be read from.
} FaultLatch;

extern FaultLatch bms_ok_latch;
extern FaultLatch imd_ok_latch;
extern FaultLatch bspd_ok_latch;
#endif

/**
 * Write a fault.
 * @param latch Fault latch instance.
 * @param status whether or not to set a fault
 */
void io_faultLatch_setCurrentStatus(const FaultLatch *latch, FaultLatchState status);

/**
 * Return whether or not there is currently a fault.
 * @param latch Fault latch instance.
 * @returns True = there is currently NOT a fault.
 */
FaultLatchState io_faultLatch_getCurrentStatus(const FaultLatch *latch);

/**
 * Return whether or not the fault latch is OK.
 * @param latch Fault latch instance.
 * @returns True = fault has NOT been latched.
 */
FaultLatchState io_faultLatch_getLatchedStatus(const FaultLatch *latch);
