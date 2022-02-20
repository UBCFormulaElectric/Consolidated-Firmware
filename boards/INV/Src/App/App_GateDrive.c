#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "App_GateDrive.h"

struct GateDrive
{
    ExitCode (*write_config)(void);
    void (*reset_status)(void);
    void (*global_reset)(void);
    void (*write_register)(
        uint8_t stgap_register,
        uint8_t data,
        bool    use_shutdown);
    void (*read_register)(
        uint8_t  stgap_register,
        uint8_t  stgap_register_mask,
        uint8_t *receive_buffer);
    struct StgapFaults *(*read_faults)(void);
    void (*command)(uint8_t command);
    void (*set_shutdown_pin)(bool pin_val);
    bool (*get_shutdown_pin)(void);
    void (*load_pwm)(const struct PhaseValues *phase_pwm_vals);
    void (*start_pwm)(void);
    void (*stop_pwm)(void);
    void (*set_switching_freq)(uint16_t switching_freq);
    void (*set_dead_time)(uint16_t dead_time);
    bool (*get_pha_hi_diag)(void);
    bool (*get_pha_lo_diag)(void);
    bool (*get_phb_hi_diag)(void);
    bool (*get_phb_lo_diag)(void);
    bool (*get_phc_hi_diag)(void);
    bool (*get_phc_lo_diag)(void);
};

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
    void (*load_gd_pwm)(const struct PhaseValues *phase_pwm_dur),
    void (*gd_start_pwm)(void),
    void (*gd_stop_pwm)(void),
    void (*gd_set_switching_freq)(uint16_t switching_freq),
    void (*gd_set_dead_time)(uint16_t dead_time),
    bool (*gd_get_pha_hi_diag)(void),
    bool (*gd_get_pha_lo_diag)(void),
    bool (*gd_get_phb_hi_diag)(void),
    bool (*gd_get_phb_lo_diag)(void),
    bool (*gd_get_phc_hi_diag)(void),
    bool (*gd_get_phc_lo_diag)(void))
{
    struct GateDrive *gate_drive = malloc(sizeof(struct GateDrive));
    assert(gate_drive != NULL);

    gate_drive->write_config       = write_gd_config;
    gate_drive->reset_status       = reset_gd_status;
    gate_drive->global_reset       = global_reset_gd;
    gate_drive->write_register     = write_gd_register;
    gate_drive->read_register      = read_gd_register;
    gate_drive->read_faults        = read_gd_faults;
    gate_drive->command            = command_gd;
    gate_drive->set_shutdown_pin   = set_gd_shutdown_pin;
    gate_drive->get_shutdown_pin   = get_gd_shutdown_pin;
    gate_drive->load_pwm           = load_gd_pwm;
    gate_drive->start_pwm          = gd_start_pwm;
    gate_drive->stop_pwm           = gd_stop_pwm;
    gate_drive->set_switching_freq = gd_set_switching_freq;
    gate_drive->set_dead_time      = gd_set_dead_time;
    gate_drive->get_pha_hi_diag    = gd_get_pha_hi_diag;
    gate_drive->get_pha_lo_diag    = gd_get_pha_lo_diag;
    gate_drive->get_phb_hi_diag    = gd_get_phb_hi_diag;
    gate_drive->get_phb_lo_diag    = gd_get_phb_lo_diag;
    gate_drive->get_phc_hi_diag    = gd_get_phc_hi_diag;
    gate_drive->get_phc_lo_diag    = gd_get_phc_lo_diag;

    return gate_drive;
}

void App_GateDrive_Destroy(struct GateDrive *gate_drive)
{
    free(gate_drive);
}

ExitCode App_GateDrive_WriteConfig(struct GateDrive *gate_drive)
{
    return gate_drive->write_config();
}

void App_GateDrive_ResetStatus(struct GateDrive *gate_drive)
{
    gate_drive->reset_status();
}

void App_GateDrive_GlobalReset(struct GateDrive *gate_drive)
{
    gate_drive->global_reset();
}

void App_GateDrive_GetFaults(
    struct GateDrive *  gate_drive,
    struct StgapFaults *stgap_faults)
{
    *stgap_faults = *(gate_drive->read_faults());
}

bool App_GateDrive_IsFaulted(void)
{
    return Io_STGAP1AS_IsFaulted();
}

void App_GateDrive_WriteRegister(
    struct GateDrive *gate_drive,
    uint8_t           register_name,
    uint8_t           data)
{
    gate_drive->write_register(register_name, data, 1);
}

void App_GateDrive_ReadRegister(
    struct GateDrive *gate_drive,
    uint8_t           register_name,
    uint8_t           register_mask,
    uint8_t *         receive_array)
{
    gate_drive->read_register(register_name, register_mask, receive_array);
}

void App_GateDrive_Command(struct GateDrive *gate_drive, uint8_t command_name)
{
    gate_drive->command(command_name);
}

void App_GateDrive_Shutdown(struct GateDrive *gate_drive)
{
    gate_drive->set_shutdown_pin(0);
}

void App_GateDrive_Enable(struct GateDrive *gate_drive)
{
    gate_drive->set_shutdown_pin(1);
}

bool App_GateDrive_IsShutdown(struct GateDrive *gate_drive)
{
    return !gate_drive->get_shutdown_pin;
}

bool App_GateDrive_IsEnabled(struct GateDrive *gate_drive)
{
    return gate_drive->get_shutdown_pin;
}

void App_GateDrive_LoadPwm(
    struct GateDrive *  gate_drive,
    struct PhaseValues *phase_pwm_dur)
{
    gate_drive->load_pwm(phase_pwm_dur);
}

void App_GateDrive_StartPwm(struct GateDrive *gate_drive)
{
    gate_drive->start_pwm();
}

void App_GateDrive_StopPwm(struct GateDrive *gate_drive)
{
    gate_drive->stop_pwm();
}

void App_GateDrive_SetSwitchingFreq(
    struct GateDrive *gate_drive,
    uint16_t          switching_freq)
{
    gate_drive->set_switching_freq(switching_freq);
}

void App_GateDrive_SetDeadTime(struct GateDrive *gate_drive, uint16_t dead_time)
{
    gate_drive->set_dead_time(dead_time);
}

bool App_GateDrive_GetPhaHiDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_pha_hi_diag();
}

bool App_GateDrive_GetPhaLoDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_pha_lo_diag();
}

bool App_GateDrive_GetPhbHiDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_phb_hi_diag();
}

bool App_GateDrive_GetPhbLoDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_phb_lo_diag();
}

bool App_GateDrive_GetPhcHiDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_phc_hi_diag();
}

bool App_GateDrive_GetPhcLoDiag(struct GateDrive *gate_drive)
{
    return gate_drive->get_phc_lo_diag();
}
