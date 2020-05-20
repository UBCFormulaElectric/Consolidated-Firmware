#include <stdlib.h>
#include <assert.h>
#include "App_OkStatus.h"

struct OkStatus
{
    bool (*is_enabled)(void);
};

struct OkStatus *App_LatchStatus_Create(bool (*is_enabled)(void))
{
    struct OkStatus *latch_status = malloc(sizeof(struct OkStatus));
    assert(latch_status != NULL);

    latch_status->is_enabled = is_enabled;

    return latch_status;
}

void App_LatchStatus_Destroy(struct OkStatus *latch_status)
{
    free(latch_status);
}

bool App_LatchStatus_IsEnabled(const struct OkStatus *latch_status)
{
    return latch_status->is_enabled();
}
