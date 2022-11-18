
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include "../../Inc/App/App_PDMError.h"

struct PdmError
{

    // The id of this error
    enum ErrorId id;

    // The type of this error
    enum ErrorType error_type;

    // If this error is enabled or not
    bool is_set;
};

struct PdmError *App_PdmError_Create(void)
{
    struct PdmError *pdm_error = malloc(sizeof(struct PdmError));
    assert(pdm_error != NULL);

    pdm_error->id         = NUM_ERROR_IDS;
    pdm_error->error_type = NUM_ERROR_TYPES;
    pdm_error->is_set     = false;

    return pdm_error;
}

void App_PdmError_Destroy(struct PdmError *pdm_error)
{
    free(pdm_error);
}

void App_PdmError_SetErrorType(struct PdmError *pdm_error, enum ErrorType error_type)
{
    pdm_error->error_type = error_type;
}

void App_PdmError_SetId(struct PdmError *pdm_error, uint32_t id)
{
    pdm_error->id = id;
}

void App_PdmError_SetIsSet(struct PdmError *pdm_error, bool is_set)
{
    pdm_error->is_set = is_set;
}

enum ErrorType App_PdmError_GetErrorType(const struct PdmError *pdm_error)
{
    return pdm_error->error_type;
}

uint32_t App_PdmError_GetId(const struct PdmError *pdm_error)
{
    return pdm_error->id;
}

bool App_PdmError_GetIsSet(const struct PdmError *pdm_error)
{
    return pdm_error->is_set;
}

bool App_PdmError_IsCritical(const struct PdmError *pdm_error)
{
    enum ErrorType error_type = App_PdmError_GetErrorType(pdm_error);

    if (error_type == AIR_SHUTDOWN_ERROR || error_type == MOTOR_SHUTDOWN_ERROR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool App_PdmError_IsNonCritical(const struct PdmError *pdm_error)
{
    enum ErrorType error_type = App_PdmError_GetErrorType(pdm_error);

    return (error_type == NON_CRITICAL_ERROR) ? true : false;
}

bool App_PdmError_IsErrorInList(struct ErrorList *error_list, enum ErrorId error_id)
{
    for (uint32_t i = 0; i < error_list->num_errors; i++)
    {
        if (App_PdmError_GetId(error_list->errors[i]) == error_id)
        {
            return true;
        }
    }
    return false;
}


