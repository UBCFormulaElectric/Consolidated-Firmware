#define usbd_conf.h

#if defined (STM32H733xx)
    #include "h7/usbd_conf.h"
#elif defined (STM32H562xx)
    #include "h5/usbd_conf.h"
#else
    #error "Unsupported MCU: please define STM32H733xx or STM32H562xx"
#endif
