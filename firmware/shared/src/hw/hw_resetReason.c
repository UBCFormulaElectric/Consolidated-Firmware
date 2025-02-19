#include "hw_resetReason.h"
#include "main.h"
#include <app_canUtils.h>

ResetReason hw_resetReason_get(void)
{
    // All the specific reasons the microcontroller can reset:
#ifdef STM32F412Rx
    const bool low_power = RCC->CSR & RCC_CSR_LPWRRSTF;
    const bool wwdg      = RCC->CSR & RCC_CSR_WWDGRSTF;
    const bool iwdg      = RCC->CSR & RCC_CSR_IWDGRSTF;
    const bool software  = RCC->CSR & RCC_CSR_SFTRSTF;
    const bool power_on  = RCC->CSR & RCC_CSR_PORRSTF;
    const bool nrst_pin  = RCC->CSR & RCC_CSR_PINRSTF;
    const bool brown_out = RCC->CSR & RCC_CSR_BORRSTF;
#elif defined(STM32H733xx)
    const bool low_power        = RCC->RSR & RCC_RSR_LPWRRSTF;
    const bool wwdg             = RCC->RSR & RCC_RSR_WWDG1RSTF;
    const bool iwdg             = RCC->RSR & RCC_RSR_IWDG1RSTF;
    const bool software         = RCC->RSR & RCC_RSR_SFTRSTF;
    const bool power_on         = RCC->RSR & RCC_RSR_PORRSTF;
    const bool nrst_pin         = RCC->RSR & RCC_RSR_PINRSTF;
    const bool brown_out        = RCC->RSR & RCC_RSR_BORRSTF;
    const bool d2_domain_switch = RCC->RSR & RCC_RSR_D2RSTF;
    const bool d1_domain_switch = RCC->RSR & RCC_RSR_D1RSTF;
#endif

    // I think this flag sets every time the MCU resets so isn't that useful?
    // Enumerating this here just for completeness.
    // const bool _reset_occured = RCC->RSR & RCC_RSR_CPURSTF;

    ResetReason reason = RESET_REASON_NONE;
    reason |= RESET_REASON_LOW_POWER & low_power;
    reason |= RESET_REASON_WWDG & wwdg;
    reason |= RESET_REASON_IWDG & iwdg;
    reason |= RESET_REASON_SOFTWARE & software;
    reason |= RESET_REASON_POWER_ON & power_on;
    reason |= RESET_REASON_NRST_PIN & nrst_pin;
    reason |= RESET_REASON_BROWN_OUT & brown_out;

#ifdef STM32H733xx
    reason |= RESET_REASON_D2_DOMAIN_SWITCH & d2_domain_switch;
    reason |= RESET_REASON_D1_DOMAIN_SWITCH & d1_domain_switch;
#endif

    return reason;
}
