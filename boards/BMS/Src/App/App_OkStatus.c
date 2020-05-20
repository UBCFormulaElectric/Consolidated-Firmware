#include <stdlib.h>
#include <assert.h>
#include "App_OkStatus.h"

struct OkStatus
{
    bool (*is_enabled)(void);
};

struct OkStatus *App_OkStatus_Create(bool (*is_enabled)(void))
{
    struct OkStatus *ok_status = malloc(sizeof(struct OkStatus));
    assert(ok_status != NULL);

    ok_status->is_enabled = is_enabled;

    return ok_status;
}

void App_OkStatus_Destroy(struct OkStatus *ok_status)
{
    free(ok_status);
}

bool App_OkStatus_IsEnabled(const struct OkStatus *ok_status)
{
    return ok_status->is_enabled();
}
