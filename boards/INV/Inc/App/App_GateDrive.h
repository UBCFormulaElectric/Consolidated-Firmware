#pragma once

#include "App_SharedExitCode.h"
#include "control/App_ControlLoop.h"
#include "Io_STGAP1AS.h"

struct GateDrive;
struct PhaseValues;
struct StgapFaults;

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
    bool (*get_gd_shutdown_pin)(void),
    void (*load_pwm)(const struct PhaseValues *phase_pwm_vals),
    void (*start_pwm)(void),
    void (*stop_pwm)(void),
    void (*set_switching_freq)(uint16_t switching_freq),
    void (*set_dead_time)(uint16_t dead_time),
    bool (*gd_get_pha_hi_diag)(void),
    bool (*gd_get_pha_lo_diag)(void),
    bool (*gd_get_phb_hi_diag)(void),
    bool (*gd_get_phb_lo_diag)(void),
    bool (*gd_get_phc_hi_diag)(void),
    bool (*gd_get_phc_lo_diag)(void));

void     App_GateDrive_Destroy(struct GateDrive *gate_drive);
ExitCode App_GateDrive_WriteConfig(struct GateDrive *gate_drive);
void     App_GateDrive_ResetStatus(struct GateDrive *gate_drive);
void     App_GateDrive_GlobalReset(struct GateDrive *gate_drive);
void     App_GateDrive_GetFaults(
        struct GateDrive *  gate_drive,
        struct StgapFaults *stgap_faults);
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
void App_GateDrive_LoadPwm(
    struct GateDrive *  gate_drive,
    struct PhaseValues *phase_pwm_dur);
void App_GateDrive_StartPwm(struct GateDrive *gate_drive);
void App_GateDrive_StopPwm(struct GateDrive *gate_drive);
void App_GateDrive_SetSwitchingFreq(
    struct GateDrive *gate_drive,
    uint16_t          switching_freq);
void App_GateDrive_SetDeadTime(
    struct GateDrive *gate_drive,
    uint16_t          dead_time);
bool App_GateDrive_GetPhaHiDiag(struct GateDrive *gate_drive);
bool App_GateDrive_GetPhaLoDiag(struct GateDrive *gate_drive);
bool App_GateDrive_GetPhbHiDiag(struct GateDrive *gate_drive);
bool App_GateDrive_GetPhbLoDiag(struct GateDrive *gate_drive);
bool App_GateDrive_GetPhcHiDiag(struct GateDrive *gate_drive);
bool App_GateDrive_GetPhcLoDiag(struct GateDrive *gate_drive);
