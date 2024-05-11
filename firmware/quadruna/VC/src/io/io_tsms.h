#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
typedef struct
{
    const Gpio *tsms_gpio;
} TSMSConfig;
void io_tsms_init(const TSMSConfig *const in_config);
#endif

bool io_tsms_read(void);