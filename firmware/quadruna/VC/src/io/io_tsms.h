#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *tsms_gpio;
} TSMSConfig;
#else
#include "app_utils.h"
EMPTY_STRUCT(PcmConfig)
#endif

void io_tsms_init(const TSMSConfig *const in_config);

bool io_tsms_read();