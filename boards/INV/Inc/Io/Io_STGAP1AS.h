#pragma once

#include "main.h"
#include "App_SharedExitCode.h"
#include <stdint.h>
#include <stdbool.h>
#include <configs/Io_GateDriveConfig.h>

struct StgapFaults;

ExitCode Io_STGAP1AS_WriteConfiguration(void);
void     Io_STGAP1AS_ResetStatus(void);
void     Io_STGAP1AS_GlobalReset(void);
void     Io_STGAP1AS_WriteRegister(
        uint8_t stgap_register,
        uint8_t data,
        bool    use_shutdown);
void Io_STGAP1AS_ReadRegister(
    uint8_t        stgap_register,
    uint8_t        stgap_register_mask,
    uint8_t *const receive_buffer);
void                Io_STGAP1AS_Command(uint8_t command);
struct StgapFaults *Io_STGAP1AS_ReadFaults(void);
uint8_t             Io_STGAP1AS_SendReceiveByte(
                uint8_t        byte_value,
                uint8_t        crc_xor,
                uint8_t *const receive_array);
uint8_t Io_STGAP1AS_CalculateCrcTx(uint8_t input, uint8_t xor_value);
uint8_t Io_STGAP1AS_CalculateCrcRx(uint8_t input, uint8_t xor_value);
void    Io_STGAP1AS_SetShutdownPin(bool value);
bool    Io_STGAP1AS_GetShutdownPin(void);
bool    Io_STGAP1AS_GetPhaHiDiag(void);
bool    Io_STGAP1AS_GetPhaLoDiag(void);
bool    Io_STGAP1AS_GetPhbHiDiag(void);
bool    Io_STGAP1AS_GetPhbLoDiag(void);
bool    Io_STGAP1AS_GetPhcHiDiag(void);
bool    Io_STGAP1AS_GetPhcLoDiag(void);

// STGAP Device Enumeration
enum
{
    PHASE_A_LO,
    PHASE_A_HI,
    PHASE_B_LO,
    PHASE_B_HI,
    PHASE_C_LO,
    PHASE_C_HI,
};

// Contains fault values for all STGAPs
struct StgapFaults
{
    bool twn[NUM_STGAP_DEVICES];
    bool tsd[NUM_STGAP_DEVICES];
    bool uvlol[NUM_STGAP_DEVICES];
    bool uvloh[NUM_STGAP_DEVICES];
    bool sense[NUM_STGAP_DEVICES];
    bool desat[NUM_STGAP_DEVICES];
    bool ovlol[NUM_STGAP_DEVICES];
    bool ovloh[NUM_STGAP_DEVICES];
    bool asc[NUM_STGAP_DEVICES];
    bool regerrr[NUM_STGAP_DEVICES];
    bool uvlod[NUM_STGAP_DEVICES];
    bool ovlod[NUM_STGAP_DEVICES];
    bool reg_errl[NUM_STGAP_DEVICES];
    bool spi_err[NUM_STGAP_DEVICES];
    bool dt_err[NUM_STGAP_DEVICES];
};

// STGAP Registers
struct StgapRegisters
{
    uint8_t cfg1;
    uint8_t cfg2;
    uint8_t cfg3;
    uint8_t cfg4;
    uint8_t cfg5;
    uint8_t status1;
    uint8_t status2;
    uint8_t status3;
    uint8_t test1;
    uint8_t diag1cfg;
    uint8_t diag2cfg;
};

struct StgapRegisterMasks
{
    uint8_t cfg1_mask;
    uint8_t cfg2_mask;
    uint8_t cfg3_mask;
    uint8_t cfg4_mask;
    uint8_t cfg5_mask;
    uint8_t status1_mask;
    uint8_t status2_mask;
    uint8_t status3_mask;
    uint8_t test1_mask;
    uint8_t diag1cfg_mask;
    uint8_t diag2cfg_mask;
};

// STGAP STATUS1 Register
enum
{
    TWN   = 0,
    TSD   = 1,
    UVLOL = 2,
    UVLOH = 3,
    SENSE = 4,
    DESAT = 5,
    OVLOL = 6,
    OVLOH = 7,
};

// STGAP STATUS2 Register
enum
{
    ASC     = 1,
    REGERRR = 2,
};

// STGAP STATUS3 Register
enum
{
    UVLOD   = 0,
    OVLOD   = 1,
    REGERRL = 2,
    SPI_ERR = 3,
    DT_ERR  = 4,
};

// STGAP TEST1 Register
enum
{
    RCHK    = 0,
    SNSCHK  = 1,
    DESCHK  = 2,
    GONCHK  = 3,
    GOFFCHK = 4,
};

// STGAP DIAG1CFG/DIAG2CFG Register
enum
{
    TWN_DIAG                     = 0,
    TSD_DIAG                     = 1,
    ASC_DT_ERR_DIAG              = 2,
    DESAT_SENSE_DIAG             = 3,
    OVLOH_OVLOL_DIAG             = 4,
    UVLOH_UVLOL_DIAG             = 5,
    UVLOD_OVLOD_DIAG             = 6,
    SPI_ERR_REGERRL_REGERRR_DIAG = 7,
};

// STGAP Commands
struct StgapCommands
{
    uint8_t start_config;
    uint8_t stop_config;
    uint8_t no_operation;
    uint8_t write_register;
    uint8_t read_register;
    uint8_t reset_status;
    uint8_t global_reset;
};
