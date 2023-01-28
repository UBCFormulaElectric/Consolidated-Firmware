#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "Io_Efuse.h"
#include "configs/Io_EfuseConfig.h"

struct Io_Efuse
{
    float test;
};

struct Io_Efuse *Io_Efuse_Create(void)
{
    struct Io_Efuse *io_efuse = malloc(sizeof(struct Io_Efuse));
    assert(io_efuse != NULL);

    return io_efuse;
}

void Io_Efuse_Destroy(struct Io_Efuse *io_efuse)
{
    free(io_efuse);
}

void Io_Efuse_EnableChannel0(struct Io_Efuse *io_efuse)
{
    //
}

void Io_Efuse_DisableChannel0(struct Io_Efuse *io_efuse)
{
    //
}

void Io_Efuse_EnableChannel1(struct Io_Efuse *io_efuse)
{
    //
}

void Io_Efuse_DisableChannel1(struct Io_Efuse *io_efuse)
{
    //
}
/*
bool Io_Efuse_IsChannel0Enabled(struct Io_Efuse *io_efuse)
{
    //
}

bool Io_Efuse_IsChannel1Enabled(struct Io_Efuse *io_efuse)
{
    //
}

void Io_Efuse_StandbyReset(struct Io_Efuse *io_efuse)
{
    //
}

float Io_Efuse_GetChannel0Current(struct Io_Efuse *io_efuse)
{
    //
}

float Io_Efuse_GetChannel1Current(struct Io_Efuse *io_efuse)
{
    //
}
*/
