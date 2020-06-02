#include <stdlib.h>
#include <assert.h>
#include "App_BinaryStatus.h"

struct BinaryStatus
{
    bool (*is_status_active)(void);
};

struct BinaryStatus *App_BinaryStatus_Create(bool (*is_status_active)(void))
{
    struct BinaryStatus *binary_status = malloc(sizeof(struct BinaryStatus));
    assert(binary_status != NULL);

    binary_status->is_status_active = is_status_active;

    return binary_status;
}

void App_BinaryStatus_Destroy(struct BinaryStatus *binary_status)
{
    free(binary_status);
}

bool App_BinaryStatus_IsStatusActive(const struct BinaryStatus *binary_status)
{
    return binary_status->is_status_active();
}
