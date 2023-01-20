
 
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "App_SharedExitCode.h"
#include "../../Drivers/STM32F3xx_HAL_Driver/Inc/stm32f3xx_hal_spi.h"
#include "../../Drivers/CMSIS/Device/ST/STM32F3xx/Include/stm32f302xc.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "Io_Efuse.h"
#include "configs/Io_EfuseConfig.h"

struct Io_Efuse;

struct Io_Efuse *Io_Efuse_Create(

);

void Io_Efuse_EnableChannel0(struct Io_Efuse *io_efuse);

void Io_Efuse_DisableChannel0(struct Io_Efuse *io_efuse);

void Io_Efuse_EnableChannel1(struct Io_Efuse *io_efuse);

void Io_Efuse_DisableChannel1(struct Io_Efuse *io_efuse);
void Io_Efuse_IsChannel0Enabled(struct Io_Efuse *io_efuse);

void Io_Efuse_IsChannel1Enabled(struct Io_Efuse *io_efuse);
void Io_Efuse_StandbyReset(struct Io_Efuse *io_efuse);

float Io_Efuse_GetChannel0Current(struct Io_Efuse *io_efuse);

float Io_Efuse_GetChannel1Current(struct Io_Efuse *io_efuse);