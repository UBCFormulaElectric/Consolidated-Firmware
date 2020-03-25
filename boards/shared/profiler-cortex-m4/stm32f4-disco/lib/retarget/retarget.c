//-- Includes ------------------------------------------------------------------
#include "retarget_conf.h"

//-- Functions -----------------------------------------------------------------
#if defined(USE_RETARGET) || defined(USE_RETARGET_ITM)

#if defined(__GNUC__)

int _write(int fd, char* ptr, int len)
{
    (void)fd;
    int i = 0;

    while (ptr[i] && (i < len)) {
        retarget_put_char(ptr[i]);
        if (ptr[i] == '\n') {
            retarget_put_char('\r');
        }
        i++;
    }

    return len;
}

void _ttywrch(int ch)
{
    retarget_put_char(ch);
}

int _read(int file, char* ptr, int len)
{
    (void)file;
    int i = 0;

    for (/* Empty */; len > 0; --len) {
        char c = retarget_get_char();
        *ptr++ = c;
        ++i;
        if (c == '\n')
            break;
    }

    return i;
}

#endif

#endif //USE_RETARGET
