#include "App_Efuse.h"

struct Efuse_Context
{
    float (*get_channel_0_current)(void);
    float (*get_channel_1_current)(void);

    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *     nss_port;
    uint16_t           nss_pin;

    GPIO_TypeDef *fsob_port;
    uint16_t      fsob_pin;
    GPIO_TypeDef *fsb_port;
    uint16_t      fsb_pin;
    GPIO_TypeDef *current_sync_port;
    uint16_t      current_sync_pin;

    GPIO_TypeDef *channel_0_port;
    uint16_t      channel_0_pin;
    GPIO_TypeDef *channel_1_port;
    uint16_t      channel_1_pin;

    bool wdin_bit_to_set;
};

struct Efuse
{
    struct Efuse_Context (*io_efuse);
    float (*get_channel_0_current)(struct Efuse_Context *const);
    float (*get_channel_1_current)(struct Efuse_Context *const);
    void (*enable_channel_0)(const struct Efuse_Context *const);
    void (*disable_channel_0)(const struct Efuse_Context *const);
    void (*enable_channel_1)(const struct Efuse_Context *const);
    void (*disable_channel_1)(const struct Efuse_Context *const);
    bool (*in_fault_mode)(const struct Efuse_Context *const);
    bool (*in_failsafe_mode)(const struct Efuse_Context *const);
    void (*delatch_fault)(const struct Efuse_Context *const);
    float (channel_0_min_current);
    float (channel_0_max_current);
    float (channel_1_min_current);
    float (channel_1_max_current);


};

struct Efuse *App_Efuse_Create(
    struct Efuse_Context (*io_efuse),
    float (*get_channel_0_current)(struct Efuse_Context *const),
    float (*get_channel_1_current)(struct Efuse_Context *const),
    void (*enable_channel_0)(const struct Efuse_Context *const),
    void (*disable_channel_0)(const struct Efuse_Context *const),
    void (*enable_channel_1)(const struct Efuse_Context *const),
    void (*disable_channel_1)(const struct Efuse_Context *const),
    bool (*in_fault_mode)(const struct Efuse_Context *const),
    bool (*in_failsafe_mode)(const struct Efuse_Context *const),
    void (*delatch_fault)(const struct Efuse_Context *const),
    float (channel_0_min_current),
    float (channel_0_max_current),
    float (channel_1_min_current),
    float (channel_1_max_current))
{
    struct Efuse *efuse = malloc(sizeof(struct Efuse));
    assert(efuse != NULL);

    efuse->io_efuse              = io_efuse;
    efuse->get_channel_0_current = get_channel_0_current;
    efuse->get_channel_1_current = get_channel_1_current;
    efuse->enable_channel_0      = enable_channel_0;
    efuse->disable_channel_0     = disable_channel_0;
    efuse->enable_channel_1      = enable_channel_1;
    efuse->disable_channel_1     = disable_channel_1;
    efuse->in_fault_mode         = in_fault_mode;
    efuse->in_failsafe_mode      = in_failsafe_mode;
    efuse->delatch_fault         = delatch_fault;
    efuse->channel_0_min_current = channel_0_min_current;
    efuse->channel_0_max_current = channel_0_max_current;
    efuse->channel_1_min_current = channel_1_min_current;
    efuse->channel_1_max_current = channel_1_max_current;

    return efuse;
}

void App_Efuse_Destroy(struct Efuse *efuse)
{
    free(efuse);
}

void App_Efuse_EnableChannel0(struct Efuse *efuse)
{
    efuse->enable_channel_0(efuse->io_efuse);
}

void App_Efuse_DisableChannel0(struct Efuse *efuse)
{
    efuse->disable_channel_0(efuse->io_efuse);
}

void App_Efuse_EnableChannel1(struct Efuse *efuse)
{
    efuse->enable_channel_1(efuse->io_efuse);
}

void App_Efuse_DisableChannel1(struct Efuse *efuse)
{
    efuse->disable_channel_1(efuse->io_efuse);
}

bool App_Efuse_IsEfuseInFaultMode(struct Efuse *efuse)
{
    return efuse->in_fault_mode(efuse->io_efuse);
}

bool App_Efuse_IsEfuseInFailSafeMode(struct Efuse *efuse)
{
    return efuse->in_failsafe_mode(efuse->io_efuse);
}

void App_Efuse_DelatchFaults(struct Efuse *efuse)
{
    efuse->delatch_fault(efuse->io_efuse);
}

float App_Efuse_GetChannel0Current(struct Efuse *efuse)
{
    return efuse->get_channel_0_current(efuse->io_efuse);
}

float App_Efuse_GetChannel1Current(struct Efuse *efuse)
{
    return efuse->get_channel_1_current(efuse->io_efuse);
}

bool App_Efuse_InRangeCheck(float value, float min_value, float max_value)
{
    if (value < min_value && value > max_value)
        return true;
    return false;
}

bool App_Efuse_Channel0_CurrentCheck(struct Efuse *efuse)
{
    if (App_Efuse_InRangeCheck(efuse->get_channel_0_current(efuse->io_efuse), efuse->channel_0_min_current , efuse->channel_0_max_current) == true)
        return true;
    return false;
}

bool App_Efuse_Channel1_CurrentCheck(struct Efuse *efuse)
{
    if (App_Efuse_InRangeCheck(efuse->get_channel_1_current(efuse->io_efuse), efuse->channel_1_min_current , efuse->channel_1_max_current) == true)
        return true;
    return false;
}
