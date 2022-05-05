#include <assert.h>
#include <stdlib.h>
#include "App_Airs.h"

struct Airs
{
    struct BinaryStatus *air_positive;
    struct BinaryStatus *air_negative;

    void (*close_air_positive)(void);
    void (*open_air_positive)(void);
};

struct Airs *App_Airs_Create(
    bool (*is_air_positive_closed)(void),
    bool (*is_air_negative_closed)(void),
    void (*close_air_positive)(void),
    void (*open_air_positive)(void))
{
    struct Airs *airs = malloc(sizeof(struct Airs));
    assert(airs != NULL);

    airs->air_positive       = App_SharedBinaryStatus_Create(is_air_positive_closed);
    airs->air_negative       = App_SharedBinaryStatus_Create(is_air_negative_closed);
    airs->close_air_positive = close_air_positive;
    airs->open_air_positive  = open_air_positive;

    return airs;
}

void App_Airs_Destroy(struct Airs *airs)
{
    free(airs->air_positive);
    free(airs->air_negative);
    free(airs);
}

struct BinaryStatus *App_Airs_GetAirPositive(const struct Airs *const airs)
{
    return airs->air_positive;
}

struct BinaryStatus *App_Airs_GetAirNegative(const struct Airs *const airs)
{
    return airs->air_negative;
}

void App_Airs_CloseAirPositive(const struct Airs *const airs)
{
    airs->close_air_positive();
}

void App_Airs_OpenAirPositive(const struct Airs *const airs)
{
    airs->open_air_positive();
}
