#include "hw/resetReason.h"
#include "main.h"
#include <stdbool.h>

static bool        reason_read;
static ResetReason reason;

ResetReason hw_resetReason_get(void)
{
    // If the reset reason has already been determined, return it. When
    // determining the reason we clear all the reset flags, so they aren't
    // still set on the next reset if they aren't part of the actual reason.
    if (reason_read)
    {
        return reason;
    }

    // All the specific reasons the microcontroller can reset. However, there
    // are a few of these we ignore:
    //
    // - WWDGRSTF: We don't use the window watchdog peripheral
    // - LPWRRSTF: We don't use any of STM's low power modes
    // - D1RSTF/D2RSTF: I think this just says which power domains experienced
    //   the reset, and doesn't specify the reason.
    //
    // (However I've enumerated them all for completeness!)
#ifdef STM32F412Rx
    const uint32_t csr        = RCC->CSR;
    const bool     _low_power = csr & RCC_CSR_LPWRRSTF;
    const bool     _wwdg      = csr & RCC_CSR_WWDGRSTF;
    const bool     iwdg       = csr & RCC_CSR_IWDGRSTF;
    const bool     software   = csr & RCC_CSR_SFTRSTF;
    const bool     power_on   = csr & RCC_CSR_PORRSTF;
    const bool     nrst_pin   = csr & RCC_CSR_PINRSTF;
    const bool     brown_out  = csr & RCC_CSR_BORRSTF;

    // Clear reset flags.
    RCC->CSR |= RCC_CSR_RMVF;
#elif defined(STM32H733xx)
    const uint32_t rsr               = RCC->RSR;
    const bool     _low_power        = rsr & RCC_RSR_LPWRRSTF;
    const bool     _wwdg             = rsr & RCC_RSR_WWDG1RSTF;
    const bool     iwdg              = rsr & RCC_RSR_IWDG1RSTF;
    const bool     software          = rsr & RCC_RSR_SFTRSTF;
    const bool     power_on          = rsr & RCC_RSR_PORRSTF;
    const bool     nrst_pin          = rsr & RCC_RSR_PINRSTF;
    const bool     brown_out         = rsr & RCC_RSR_BORRSTF;
    const bool     _d2_domain_switch = rsr & RCC_RSR_D2RSTF;
    const bool     _d1_domain_switch = rsr & RCC_RSR_D1RSTF;

    // Clear reset flags.
    RCC->RSR |= RCC_RSR_RMVF;
#endif

    // This order matters! Empirically, I've seen that:
    //
    // - If there's a watchdog reset, the IWDGRSTF and PINNRSTF flags are set
    // - If there's a debugger reset, the SFTRSTF flag and PINNRSTF flags are
    //   set
    //
    // I've put PINNRSTF as the lowest priority so that a watchdog reset is
    // correctly identified, and a debugger reset counts as a software reset.

    if (iwdg)
    {
        reason = RESET_REASON_WATCHDOG;
    }
    else if (software)
    {
        reason = RESET_REASON_SOFTWARE;
    }
    else if (power_on)
    {
        reason = RESET_REASON_POWER_ON;
    }
    else if (brown_out)
    {
        reason = RESET_REASON_BROWN_OUT;
    }
    else if (nrst_pin)
    {
        reason = RESET_REASON_NRST_PIN;
    }
    else
    {
        reason = RESET_REASON_UNKNOWN;
    }

    reason_read = true;
    return reason;
}
