#pragma once

#include <cstdint>
#ifdef TARGET_EMBEDDED
#include "hw_gpios.hpp"
#endif

namespace io
{
struct FaultLatch
{
    enum class FaultLatchState : uint8_t
    {
        FAULT = 0,
        OK    = 1,
    };
#ifdef TARGET_EMBEDDED
    const hw::gpio &current_status_gpio;
    const hw::gpio &latch_status_gpio;
    bool            current_inverted;
    bool            latch_inverted;
#else
    FaultLatchState status;
    FaultLatchState latched_state;
#endif
    const bool read_only; // Certain fault latches can only be read from.

    /**
     * Write a fault.
     * @param status whether or not to set a fault
     */
    void setCurrentStatus(FaultLatchState status) const;

    /**
     * Return whether or not there is currently a fault.
     * @returns True = there is currently NOT a fault.
     */
    FaultLatchState getCurrentStatus() const;

    /**
     * Return whether or not the fault latch is OK.
     * @returns True = fault has NOT been latched.
     */
    FaultLatchState getLatchedStatus() const;
};
} // namespace io

#ifdef TARGET_EMBEDDED
extern const io::FaultLatch bms_ok_latch;
extern const io::FaultLatch imd_ok_latch;
extern const io::FaultLatch bspd_ok_latch;
#else
extern io::FaultLatch bms_ok_latch;
extern io::FaultLatch imd_ok_latch;
extern io::FaultLatch bspd_ok_latch;
#endif
