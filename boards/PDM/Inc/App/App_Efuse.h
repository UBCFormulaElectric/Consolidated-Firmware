#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "App_Efuse.h"

struct Efuse;

struct Efuse *App_Efuse_Create(
    float (*get_channel_0_current)(void),
    float (*get_channel_1_current)(void),
    bool (*is_pin_0_on)(void),
    bool (*is_pin_1_on)(void),
    bool (*is_pin_stby_on)(void));

void App_EfuseDestroy(struct Efuse *efuse);

bool App_EfuseEnableorDisableChannel0(struct Efuse *efuse, bool (*function)(bool));
bool App_EfuseEnableorDisableChannel1(struct Efuse *efuse, bool (*function)(bool));
bool App_EfuseEnableorDisableSTBYChannel(struct Efuse *efuse, bool (*function)(bool));

float App_Efuse_GetChannel0Current(struct Efuse *efuse);
float App_Efuse_GetChannel1Current(struct Efuse *efuse);

int   App_Efuse_InRangeCheck(float value, float min_value, float max_value);

bool App_Efuse_Channel0_CurrentCheck(struct Efuse *efuse, float min_value, float max_value);
bool App_Efuse_Channel1_CurrentCheck(struct Efuse *efuse, float min_value, float max_value);
