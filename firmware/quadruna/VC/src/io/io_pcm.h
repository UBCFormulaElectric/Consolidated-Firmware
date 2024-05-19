#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *pcm_gpio;
} PcmConfig;
void io_pcm_init(const PcmConfig *const in_config);
#endif

void io_pcm_set(bool val);