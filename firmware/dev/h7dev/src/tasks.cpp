#include "tasks.h"

extern "C"
{
#include "SEGGER_SYSVIEW.h"
#include "io_log.h"
}

static int write_num = 0;
static int read_num  = 0;

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
}

void tasks_default()
{
    for (;;)
    {
;
    }
}

void tasks_canRx()
{
    for (;;)
    {

    }
}

void tasks_canTx()
{
    for (;;)
    {

    }
}
