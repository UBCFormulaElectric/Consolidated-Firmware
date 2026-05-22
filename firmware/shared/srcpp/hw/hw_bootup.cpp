#include "hw_bootup.hpp"

volatile __attribute__((section(".boot_request"))) hw::bootup::BootRequestData hw::bootup::boot_request;

void hw::bootup::setBootRequest(const BootRequest request)
{
    boot_request.magic                 = BOOT_MAGIC;
    boot_request.request.target        = request.target;
    boot_request.request.context       = request.context;
    boot_request.request._unused       = request._unused;
    boot_request.request.context_value = request.context_value;
}

hw::bootup::BootRequest hw::bootup::getBootRequest()
{
    if (boot_request.magic == BOOT_MAGIC)
    {
        return {
            .target        = boot_request.request.target,
            .context       = boot_request.request.context,
            ._unused       = boot_request.request._unused,
            .context_value = boot_request.request.context_value,
        };
    }
    // Default to app if magic not present.
    return {
        .target        = BootTarget::APP,
        .context       = BootContext::NONE,
        ._unused       = 0xFFFF,
        .context_value = 0,
    };
}
