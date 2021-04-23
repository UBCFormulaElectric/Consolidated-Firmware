#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "App_GateDrive.h"

struct GateDrive
{
	void (*write_config)(void);
	void (*reset_status)(void);
	void (*global_reset)(void);
	void (*clear_faults)(void);
	void (*write_register)(uint8_t stgap_register, uint8_t data, bool use_shutdown);
	void (*read_register)(uint8_t stgap_register, uint8_t stgap_register_mask, uint8_t * receive_buffer);
	struct StgapFaults * (*read_faults)(void);
	void (*command)(uint8_t command);
	void (*set_shutdown_pin)(bool pin_val);
	bool (*get_shutdown_pin)(void);
};

struct GateDrive *App_GateDrive_Create(
		void (*write_gd_config)(void),
		void (*reset_gd_status)(void),
		void (*global_reset_gd)(void),
		void (*clear_gd_faults)(void),
		void (*write_gd_register)(uint8_t stgap_register, uint8_t data, bool use_shutdown),
		void (*read_gd_register)(uint8_t stgap_register, uint8_t stgap_register_mask, uint8_t * receive_buffer),
		struct StgapFaults * (*read_gd_faults)(void),
		void (*command_gd)(uint8_t command),
		void (*set_gd_shutdown_pin)(bool pin_val),
		bool (*get_gd_shutdown_pin)(void))
{
    struct GateDrive *gate_drive = malloc(sizeof(struct GateDrive));
    assert(gate_drive != NULL);

    gate_drive->write_config = write_gd_config;
    gate_drive->reset_status = reset_gd_status;
    gate_drive->global_reset = global_reset_gd;
    gate_drive->clear_faults = clear_gd_faults;
    gate_drive->write_register = write_gd_register;
    gate_drive->read_register = read_gd_register;
    gate_drive->read_faults = read_gd_faults;
    gate_drive->command = command_gd;
	gate_drive->set_shutdown_pin     = set_gd_shutdown_pin;
	gate_drive->get_shutdown_pin	= get_gd_shutdown_pin;

    return gate_drive;
}

void App_GateDrive_Destroy(struct GateDrive *gate_drive)
{
	free(gate_drive);
}

void App_GateDrive_WriteConfig(struct GateDrive *gate_drive)
{
	gate_drive->write_config;
}

void App_GateDrive_ResetStatus(struct GateDrive *gate_drive)
{
	gate_drive->reset_status;
}

void App_GateDrive_GlobalReset(struct GateDrive *gate_drive)
{
	gate_drive->global_reset;
}

void App_GateDrive_ClearFaults(struct GateDrive *gate_drive)
{
	gate_drive->clear_faults;
}

void App_GateDrive_WriteRegister(struct GateDrive *gate_drive, uint8_t register_name, uint8_t data)
{
	gate_drive->write_register(register_name, data, 1);
}

void App_GateDrive_ReadRegister(struct GateDrive *gate_drive, uint8_t register_name, uint8_t register_mask, uint8_t * receive_array)
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
