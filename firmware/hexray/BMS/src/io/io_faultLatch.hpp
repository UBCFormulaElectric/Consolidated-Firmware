#pragma once

// I have made this compatible with booleans, don't make me regret it

#include <cstdint>

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#endif

namespace io::faultLatch
{

enum class FaultLatchState : std::uint8_t
{
    FAULT = 0,
    OK    = 1,
};

#ifdef TARGET_EMBEDDED

struct FaultLatch
{
    const Gpio *current_status_gpio;
    const Gpio *latch_status_gpio;
    bool        inverted;
    const bool  read_only; // Certain fault latches can only be read from.
};

extern const FaultLatch bms_ok_latch;
extern const FaultLatch imd_ok_latch;
extern const FaultLatch bspd_ok_latch;

#else

struct FaultLatch
{
    FaultLatchState status;
    FaultLatchState latched_state;
    bool            inverted;
    const bool      read_only; // Certain fault latches can only be read from.
};

extern FaultLatch bms_ok_latch;
extern FaultLatch imd_ok_latch;
extern FaultLatch bspd_ok_latch;

#endif

/**
 * Write a fault.
 * @param latch Fault latch instance.
 * @param status whether or not to set a fault
 */
void setCurrentStatus(const FaultLatch *latch, FaultLatchState status);

/**
 * Return whether or not there is currently a fault.
 * @param latch Fault latch instance.
 * @returns True = there is currently NOT a fault.
 */
FaultLatchState getCurrentStatus(const FaultLatch *latch);

/**
 * Return whether or not the fault latch is OK.
 * @param latch Fault latch instance.
 * @returns True = fault has NOT been latched.
 */
FaultLatchState getLatchedStatus(const FaultLatch *latch);
} // namespace io::faultLatch