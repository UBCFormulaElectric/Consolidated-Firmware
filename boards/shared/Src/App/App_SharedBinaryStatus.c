#include <stdlib.h>
#include <assert.h>
#include "App_SharedBinaryStatus.h"

struct SharedBinaryStatus
{
    bool (*is_status_active)(void);
};

struct SharedBinaryStatus *
    App_SharedBinaryStatus_Create(bool (*is_status_active)(void))
{
    struct SharedBinaryStatus *binary_status =
        malloc(sizeof(struct SharedBinaryStatus));
    assert(binary_status != NULL);

    binary_status->is_status_active = is_status_active;

    return binary_statu;
}

void App_SharedBinaryStatus_Destroy(struct SharedBinaryStatus *binary_status)
{
    free(binary_status);
}

bool App_SharedBinaryStatus_IsStatusActive(
    const struct SharedBinaryStatus *binary_status)
{
    return binary_status->is_status_active();
}
