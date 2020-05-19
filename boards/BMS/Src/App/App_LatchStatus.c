#include <stdlib.h>
#include <assert.h>
#include "App_LatchStatus.h"

struct LatchStatus
{
    bool (*is_enabled)(void);
};

struct LatchStatus *App_LatchStatus_Create(bool (*is_enabled)(void))
{
    struct LatchStatus *latch = malloc(sizeof(struct LatchStatus));
    assert(latch != NULL);

    latch->is_enabled = is_enabled;

    return latch;
}

bool App_LatchStatus_IsEnabled(const struct LatchStatus *latch)
{
    return latch->is_enabled();
}
