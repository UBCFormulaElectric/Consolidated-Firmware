#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *pcm_gpio;
} PcmConfig;

/**
 * Initialize the IO PCM module.
 * @param in_config PCM config struct.
 */
void io_pcm_init(const PcmConfig *const in_config);
#endif

/**
 * Enable the PCM.
 * @param enable Whether or not to enable the PCM.
 */
void io_pcm_set(bool enable);