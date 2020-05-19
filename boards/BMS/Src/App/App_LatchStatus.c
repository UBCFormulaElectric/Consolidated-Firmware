#include <stdlib.h>
#include <assert.h>
#include "App_LatchStatus.h"

struct LatchStatus
{
    bool (*is_enabled)(void);
};

struct LatchStatus *App_LatchStatus_Create(bool (*is_enabled)(void))
{
    struct LatchStatus *latch_status = malloc(sizeof(struct LatchStatus));
    assert(latch_status != NULL);

    latch_status->is_enabled = is_enabled;

    return latch_status;
}

void App_LatchStatus_Destroy(struct LatchStatus *latch_status)
{
    free(latch_status);
}

bool App_LatchStatus_IsEnabled(const struct LatchStatus *latch_status)
{
    return latch_status->is_enabled();
}
