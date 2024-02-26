#pragma once

#include "logfs.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define CHECK_ERR(err) (err != LOGFS_ERR_OK)

#define RET_ERR(err)    \
    if (CHECK_ERR(err)) \
    {                   \
        return err;     \
    }