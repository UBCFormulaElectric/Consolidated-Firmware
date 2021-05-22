#pragma once

struct GateDrive;

#include "App_SharedExitCode.h"
#include "Io_STGAP1AS.h"

struct GateDrive *App_GateDrive_Create(
    ExitCode (*write_gd_config)(void),
    void (*reset_gd_status)(void),
    void (*global_reset_gd)(void),
    void (*write_gd_register)(
        uint8_t stgap_register,
        uint8_t data,
        bool    use_shutdown),
    void (*read_gd_register)(
        uint8_t  stgap_register,
        uint8_t  stgap_register_mask,
        uint8_t *receive_buffer),
    struct StgapFaults *(*read_gd_faults)(void),
    void (*command_gd)(uint8_t command),
    void (*set_gd_shutdown_pin)(bool pin_val),
    bool (*get_gd_shutdown_pin)(void));

void App_GateDrive_Destroy(struct GateDrive *gate_drive);
ExitCode App_GateDrive_WriteConfig(struct GateDrive *gate_drive);
void App_GateDrive_ResetStatus(struct GateDrive *gate_drive);
void App_GateDrive_GlobalReset(struct GateDrive *gate_drive);
void App_GateDrive_ClearFaults(struct GateDrive *gate_drive);
void App_GateDrive_WriteRegister(
    struct GateDrive *gate_drive,
    uint8_t           register_name,
    uint8_t           data);
void App_GateDrive_ReadRegister(
    struct GateDrive *gate_drive,
    uint8_t           register_name,
    uint8_t           register_mask,
    uint8_t *         receive_array);
void App_GateDrive_Command(struct GateDrive *gate_drive, uint8_t command_name);
void App_GateDrive_Shutdown(struct GateDrive *gate_drive);
void App_GateDrive_Enable(struct GateDrive *gate_drive);
bool App_GateDrive_IsShutdown(struct GateDrive *gate_drive);
bool App_GateDrive_IsEnabled(struct GateDrive *gate_drive);
