#include "hw_bootup.hpp"
#include "hw_hal.hpp"
#include "FreeRTOS.h"


volatile __attribute__((section(".boot_request"))) hw::bootup::BootRequestData hw::bootup::boot_request;;

void hw::bootup::setBootRequest(const hw::bootup::BootRequest request)
{
    hw::bootup::boot_request.magic                 = BOOT_MAGIC;
    hw::bootup::boot_request.request.target        = request.target;
    hw::bootup::boot_request.request.context       = request.context;
    hw::bootup::boot_request.request._unused       = request._unused;
    hw::bootup::boot_request.request.context_value = request.context_value;
}

hw::bootup::BootRequest hw::bootup::getBootRequest()
{
    if (boot_request.magic == BOOT_MAGIC)
    {
        hw::bootup::BootRequest r;
        r.target        = hw::bootup::boot_request.request.target;
        r.context       = hw::bootup::boot_request.request.context;
        r._unused       = hw::bootup::boot_request.request._unused;
        r.context_value = hw::bootup::boot_request.request.context_value;

        return r;
    }
    else
    {
        // Default to app if magic not present.
        const hw::bootup::BootRequest request = {
            .target        = hw::bootup::BootTarget::BOOT_TARGET_APP,
            .context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE,
            ._unused       = 0xFFFF,
            .context_value = 0,
        };
        return request;
    }
}
