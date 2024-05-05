#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *pcm_gpio;
} PcmConfig;
#else
#include "app_utils.h"
EMPTY_STRUCT(PcmConfig)
#endif

void io_pcm_init(const PcmConfig *const in_config);

void io_pcm_set(bool val);