#include <stdlib.h>
#include <assert.h>
#include "App_SharedBinaryStatus.h"

struct BinaryStatus
{
    bool (*is_status_active)(void);
};

struct BinaryStatus *App_SharedBinaryStatus_Create(bool (*is_status_active)(void))
{
    struct BinaryStatus *binary_status = malloc(sizeof(struct BinaryStatus));
    assert(binary_status != NULL);

    binary_status->is_status_active = is_status_active;

    return binary_status;
}

void App_SharedBinaryStatus_Destroy(struct BinaryStatus *binary_status)
{
    free(binary_status);
}

bool App_SharedBinaryStatus_IsActive(const struct BinaryStatus *binary_status)
{
    return binary_status->is_status_active();
}
