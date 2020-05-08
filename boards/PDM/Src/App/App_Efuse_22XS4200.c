#include "App_Efuse_22XS4200.h"

#define CSB_PORT(E_FUSE) CSB_##E_FUSE##GPIO_PORT
#define CSB_PIN(E_FUSE) CSB_##E_FUSE##GPIO_Pin
#define CUR_SYNC_PORT(E_FUSE) CUR_SYNC_##E_FUSE##GPIO_PORT
#define CUR_SYNC_PIN(E_FUSE) CUR_SYNC_##E_FUSE##GPIO_Pin

struct E_Fuses
{
    struct E_Fuse *e_fuses[NUM_E_FUSES];
};

struct E_Fuse *App_E_Fuse_Create(
    GPIO_TypeDef *e_fuse_cs_GPIO_Port,
    uint16_t      e_fuse_cs_GPIO_Pin,
    GPIO_TypeDef *e_fuse_cur_sync_GPIO_Port,
    uint16_t      e_fuse_cur_sync_GPIO_Pin)
{
    struct E_Fuse *e_fuse = malloc(sizeof(struct E_Fuse));

    assert(e_fuse != NULL);

    e_fuse->watch_dog_state              = true;
    e_fuse->status                       = StatusType_NoFault;
    e_fuse->faults.channel_0_faults      = FaultType_NoFault;
    e_fuse->faults.channel_1_faults      = FaultType_NoFault;
    e_fuse->chip_select.GPIO_Port        = e_fuse_cs_GPIO_Port;
    e_fuse->chip_select.GPIO_Pin         = e_fuse_cs_GPIO_Pin;
    e_fuse->current_sense_sync.GPIO_Port = e_fuse_cur_sync_GPIO_Port;
    e_fuse->current_sense_sync.GPIO_Pin  = e_fuse_cur_sync_GPIO_Pin;
    return e_fuse;
}

void App_E_Fuse_Destroy(struct E_Fuse *const e_fuse)
{
    free(e_fuse);
}

struct E_Fuses *App_E_Fuses_Create(
    struct E_Fuse *const e_fuse_1,
    struct E_Fuse *const e_fuse_2,
    struct E_Fuse *const e_fuse_3,
    struct E_Fuse *const e_fuse_4)
{
    struct E_Fuses *e_fuses = malloc(sizeof(struct E_Fuses));

    assert(e_fuses != NULL);

    e_fuses->e_fuses[AUX1__AUX2_E_FUSE]                    = e_fuse_1;
    e_fuses->e_fuses[ENERGY_METER__COOLING_E_FUSE]         = e_fuse_2;
    e_fuses->e_fuses[LEFT_INVERTER__RIGHT_INVERTER_E_FUSE] = e_fuse_3;
    e_fuses->e_fuses[AIR_SHUTDOWN__CAN_POWER_E_FUSE]       = e_fuse_4;

    return e_fuses;
}

void App_E_Fuses_Destroy(struct E_Fuses *const e_fuses)
{
    free(e_fuses);
}

HAL_StatusTypeDef App_Efuse_22XS4200_ExitFailSafe(struct E_Fuse *e_fuse)
{
    HAL_StatusTypeDef status = HAL_OK;

    // Set WDIN bit for next write
    // 1_1_00000_00000_0000
    e_fuse->watch_dog_state = true;

    Io_Efuse_22XS4200_WriteReg(SI_STATR_0_ADDR, 0x0000, e_fuse);
    // Disable watchdog
    Io_Efuse_22XS4200_WriteReg(SI_GCR_ADDR, GCR_CONFIG, e_fuse);
    return status;
}

void App_Efuse_22XS4200_configureEfuses(struct E_Fuses *e_fuses)
{
    for (uint8_t i = 0; i < NUM_E_FUSES; i++)
    {
        Io_Efuse_22XS4200_configureEfuse(e_fuses->e_fuses[i]);
    }
}

void App_Efuse_22XS4200_UpdateEfusesStatus(struct E_Fuses *e_fuses)
{
    for (uint8_t i = 0; i < NUM_E_FUSES; i++)
    {
        Io_Efuse_22XS4200_UpdateStatus(e_fuses->e_fuses[i]);
    }
}

void App_Efuse_22XS4200_UpdateEfusesFaults(struct E_Fuses *e_fuses)
{
    for (uint8_t i = 0; i < NUM_E_FUSES; i++)
    {
        Io_Efuse_22XS4200_UpdateFaults(e_fuses->e_fuses[i]);
    }
}
