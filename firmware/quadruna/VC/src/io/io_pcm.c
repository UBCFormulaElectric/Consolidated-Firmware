#include "io_pcm.h"

static const PcmConfig * config;

void io_pcm_init(const PcmConfig *const in_config) {
    config = in_config;
}

void io_pcm_set(bool val) {
    hw_gpio_writePin(config->pcm_gpio, val);
}