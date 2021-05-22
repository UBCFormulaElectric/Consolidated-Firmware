/*
    Daisy Chain Device Order:
    PHA_HI, PHA_LO, PHB_HI, PHB_LO, PHC_HI, PHC_LO

    CRC Examples: 			CMD Byte	CMD CRC		Data Byte	Data CRC
    StopConfig 				0x3A 		0xAA 		N.A. 		N.A.
    WriteReg(CFG1, 0x20) 	0x8C 		0xA1 		0x20 		0x82
    WriteReg(CFG5, 0x06) 	0x99 		0xCA 		0x06 		0x66
    ResetStatus 			0xD0 		0x32 		N.A. 		N.A.
    ReadReg(CFG3) 			0xBE 		0x3F 		N.A. 		N.A.
 */

#include "Io_STGAP1AS.h"
#include "main.h"
#include <stdint.h>

SPI_HandleTypeDef *stgap_spi_handle;

static const uint8_t crc_table[UINT8_MAX + 1] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
    0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
    0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
    0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
    0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
    0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
    0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
    0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
    0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
    0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
    0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
    0xFA, 0xFD, 0xF4, 0xF3,
};

struct StgapFaults stgap_faults;

struct StgapRegisters stgap_registers = {
    .cfg1     = 0x0C,
    .cfg2     = 0x1D,
    .cfg3     = 0x1E,
    .cfg4     = 0x1F,
    .cfg5     = 0x19,
    .status1  = 0x02,
    .status2  = 0x01,
    .status3  = 0x0A,
    .test1    = 0x11,
    .diag1cfg = 0x05,
    .diag2cfg = 0x06,
};

struct StgapRegisterMasks stgap_register_masks = {
    .cfg1_mask     = 0xFF,
    .cfg2_mask     = 0xFF,
    .cfg3_mask     = 0xFF,
    .cfg4_mask     = 0x3F,
    .cfg5_mask     = 0x0F,
    .status1_mask  = 0xFF,
    .status2_mask  = 0x06,
    .status3_mask  = 0x1F,
    .test1_mask    = 0x1F,
    .diag1cfg_mask = 0xFF,
    .diag2cfg_mask = 0xFF,
};

struct StgapCommands stgap_commands = {
    .start_config   = 0x3A,
    .stop_config    = 0x8C,
    .no_operation   = 0x00,
    .write_register = 0x80,
    .read_register  = 0xA0,
    .reset_status   = 0xD0,
    .global_reset   = 0xEA,
};

uint8_t config1_register_content = (CRC_SPI << 7) | (UVLOD_EN << 6) |
                                   (SD_FLAG << 5) | (DIAG_EN << 4) |
                                   (DT_SET << 2) | (IN_FILTER);

uint8_t config2_register_content =
        (SENSE_THRES << 5) | (DESAT_CURR << 3) | (DESAT_THRES);

uint8_t config3_register_content = (TLTO_THRES << 4) | (TLTO_TIME);

uint8_t config4_register_content =
    (OVLO_EN << 5) | (UVLO_LATCH << 4) | (VLON_THRES << 2) | (VHON_THRES);

uint8_t config5_register_content =
    (TLTO_EN << 3) | (SENSE_EN << 2) | (DESAT_EN << 1) | (CLAMP_EN);

uint8_t diag1cfg_register_content = (DIAG1_TWN << 7) | (DIAG1_TSD << 6) |
                                    (DIAG1_ASC << 5) | (DIAG1_DESAT << 4) |
                                    (DIAG1_OV << 3) | (DIAG1_UV << 2) |
                                    (DIAG1_VDD << 1) | (DIAG1_SPI_REG_ERR);

uint8_t diag2cfg_register_content = (DIAG2_TWN << 7) | (DIAG2_TSD << 6) |
                                    (DIAG2_ASC << 5) | (DIAG2_DESAT << 4) |
                                    (DIAG2_OV << 3) | (DIAG2_UV << 2) |
                                    (DIAG2_VDD << 1) | (DIAG2_SPI_REG_ERR);

void Io_STGAP1AS_Init(SPI_HandleTypeDef *const handle)
{
    stgap_spi_handle = handle;
}

ExitCode Io_STGAP1AS_WriteConfiguration(void)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES * 7];

    // Reset status, and start configuration
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.reset_status);
    Io_STGAP1AS_Command(stgap_commands.start_config);

    // Write Configuration & Diagnostic Registers
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.cfg1,
        config1_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.cfg2,
        config2_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.cfg3,
        config3_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.cfg4,
        config4_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.cfg5,
        config5_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.diag1cfg,
        diag1cfg_register_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_commands.write_register | stgap_registers.diag2cfg,
        diag2cfg_register_content, 0);

    // Read Configuration & Diagnostic Registers
    Io_STGAP1AS_ReadRegister(
        stgap_registers.cfg1, stgap_register_masks.cfg1_mask,
        (uint8_t*)&receive_buffer);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i] != stgap_registers.cfg1)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.cfg2, stgap_register_masks.cfg2_mask,
        (uint8_t*)(&receive_buffer) + 6);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 6] != stgap_registers.cfg2)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.cfg3, stgap_register_masks.cfg3_mask,
        (uint8_t*)(&receive_buffer + 12));
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 12] != stgap_registers.cfg3)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.cfg4, stgap_register_masks.cfg4_mask,
        (uint8_t*)(&receive_buffer) + 18);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 18] != stgap_registers.cfg4)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.cfg5, stgap_register_masks.cfg5_mask,
        (uint8_t*)(&receive_buffer) + 24);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 24] != stgap_registers.cfg5)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.diag1cfg, stgap_register_masks.diag1cfg_mask,
        (uint8_t*)(&receive_buffer) + 30);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 30] != stgap_registers.diag1cfg)
        {
            return EXIT_CODE_ERROR;
        }
    }
    Io_STGAP1AS_ReadRegister(
        stgap_registers.diag2cfg, stgap_register_masks.diag2cfg_mask,
        (uint8_t*)(&receive_buffer) + 36);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i + 36] != stgap_registers.diag2cfg)
        {
            return EXIT_CODE_ERROR;
        }
    }

    // Stop configuration
    Io_STGAP1AS_Command(stgap_commands.stop_config);
    Io_STGAP1AS_SetShutdownPin(1);

    return EXIT_CODE_OK;
}

void Io_STGAP1AS_ResetStatus(void)
{
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.reset_status);
    Io_STGAP1AS_SetShutdownPin(1);
}

void Io_STGAP1AS_GlobalReset(void)
{
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.start_config);
    Io_STGAP1AS_Command(stgap_commands.global_reset);
    Io_STGAP1AS_Command(stgap_commands.stop_config);
    Io_STGAP1AS_SetShutdownPin(1);
}

bool Io_STGAP1AS_GonPathCheck(void)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES];
    uint8_t test1_content = (0 << GOFFCHK) | (1 << GONCHK) | (0 << DESCHK) |
                            (0 << SNSCHK) | (0 << RCHK);

    // Gon path check test procedure
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.start_config);
    Io_STGAP1AS_WriteRegister(stgap_registers.test1, test1_content, 0);
    Io_STGAP1AS_Command(stgap_commands.stop_config);
    // TODO wait some time here, vTaskDelay(1), may go after shutdown;
    Io_STGAP1AS_SetShutdownPin(1);
    Io_STGAP1AS_ReadRegister(
        stgap_registers.status1, stgap_register_masks.status1_mask,
        (uint8_t*)&receive_buffer);

    //Check all device status registers for faults
    for(uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i] >> TSD & 1)
        {
            // TODO failure code here
            return 0;
        }
    }
    return 1;
}

bool Io_STGAP1AS_GoffPathCheck(void)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES];
    uint8_t test1_content = (1 << GOFFCHK) | (0 << GONCHK) | (0 << DESCHK) |
                            (0 << SNSCHK) | (0 << RCHK);

    // Goff path check test procedure
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.start_config);
    Io_STGAP1AS_WriteRegister(stgap_registers.test1, test1_content, 0);
    Io_STGAP1AS_Command(stgap_commands.stop_config);
    // TODO wait some time here, vTaskDelay(1), may go after shutdown;
    Io_STGAP1AS_SetShutdownPin(1);
    Io_STGAP1AS_ReadRegister(
        stgap_registers.status1, stgap_register_masks.status1_mask,
        (uint8_t*)&receive_buffer);

    //Check all device status registers for faults
    for(uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i] >> DESAT & 1)
        {
            // TODO failure code here
            return 0;
        }
    }
    return 1;
}

bool Io_STGAP1AS_DesatCompCheck(void)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES];
    uint8_t test1_content = (0 << GOFFCHK) | (0 << GONCHK) | (1 << DESCHK) |
                            (0 << SNSCHK) | (0 << RCHK);

    // Desat comparator check test procedure
    Io_STGAP1AS_SetShutdownPin(0);
    Io_STGAP1AS_Command(stgap_commands.start_config);
    Io_STGAP1AS_WriteRegister(stgap_registers.test1, test1_content, 0);
    Io_STGAP1AS_WriteRegister(
        stgap_registers.cfg5, config5_register_content, 0);
    Io_STGAP1AS_Command(stgap_commands.stop_config);
    Io_STGAP1AS_SetShutdownPin(1);
    // TODO Wait 3 µs
    // TODO Apply pulse at gate longer than 500ns
    Io_STGAP1AS_ReadRegister(
        stgap_registers.status1, stgap_register_masks.status1_mask,
        (uint8_t*)&receive_buffer);

    //Check all device status registers for faults
    for(uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        if (receive_buffer[i] >> DESAT & 1)
        {
            // TODO failure code here
            return 0;
        }
    }
    return 1;
}

void Io_STGAP1AS_WriteRegister(
    const uint8_t stgap_register,
    const uint8_t data,
    const bool    use_shutdown)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES];
    uint8_t calculated_crc;
    if (use_shutdown)
    {
        Io_STGAP1AS_SetShutdownPin(0);
        Io_STGAP1AS_SendReceiveByte(stgap_commands.start_config, 0xFF, (uint8_t*)&receive_buffer);
    }

    calculated_crc = Io_STGAP1AS_SendReceiveByte(
        stgap_commands.write_register | stgap_register, 0xFF, (uint8_t*)&receive_buffer);
    Io_STGAP1AS_SendReceiveByte(data, calculated_crc, (uint8_t*)&receive_buffer);

    if (use_shutdown)
    {
        Io_STGAP1AS_SendReceiveByte(stgap_commands.stop_config, 0xFF, (uint8_t*)&receive_buffer);
        Io_STGAP1AS_SetShutdownPin(1);
    }
    //TODO could return the readback register here if needed. size of receive_buffer will need to be modified.
}

void Io_STGAP1AS_ReadRegister(
    const uint8_t        stgap_register,
    const uint8_t        stgap_register_mask,
    uint8_t *const receive_buffer)
{
    Io_STGAP1AS_SendReceiveByte(
        stgap_commands.read_register | stgap_register, 0xFF, receive_buffer);
    Io_STGAP1AS_SendReceiveByte(stgap_commands.no_operation, 0xFF, receive_buffer);
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        *(receive_buffer + i) = *(receive_buffer + i) & stgap_register_mask;
    }

}

void Io_STGAP1AS_Command(const uint8_t command)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES];
    Io_STGAP1AS_SendReceiveByte(command, 0xFF, receive_buffer);
}

struct StgapFaults* Io_STGAP1AS_ReadFaults()
{
    uint8_t receive_array[3*NUM_STGAP_DEVICES];
    Io_STGAP1AS_ReadRegister(stgap_registers.status1, stgap_register_masks.status1_mask, (uint8_t*)&receive_array + 0*NUM_STGAP_DEVICES);
    Io_STGAP1AS_ReadRegister(stgap_registers.status2, stgap_register_masks.status2_mask, (uint8_t*)&receive_array + 1*NUM_STGAP_DEVICES);
    Io_STGAP1AS_ReadRegister(stgap_registers.status3, stgap_register_masks.status3_mask, (uint8_t*)&receive_array + 2*NUM_STGAP_DEVICES);

    for(uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        stgap_faults.twn[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 0 & 1);
        stgap_faults.tsd[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 1 & 1);
        stgap_faults.uvlol[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 2 & 1);
        stgap_faults.uvloh[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 3 & 1);
        stgap_faults.sense[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 4 & 1);
        stgap_faults.desat[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 5 & 1);
        stgap_faults.ovlol[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 6 & 1);
        stgap_faults.ovloh[i] = (bool)(receive_array[0*NUM_STGAP_DEVICES + i] >> 7 & 1);
        stgap_faults.asc[i] = (bool)(receive_array[1*NUM_STGAP_DEVICES + i] >> 1 & 1);
        stgap_faults.regerrr[i] = (bool)(receive_array[1*NUM_STGAP_DEVICES + i] >> 2 & 1);
        stgap_faults.uvlod[i] = (bool)(receive_array[2*NUM_STGAP_DEVICES + i] >> 0 & 1);
        stgap_faults.ovlod[i] = (bool)(receive_array[2*NUM_STGAP_DEVICES + i] >> 1 & 1);
        stgap_faults.reg_errl[i] = (bool)(receive_array[2*NUM_STGAP_DEVICES + i] >> 2 & 1);
        stgap_faults.spi_err[i] = (bool)(receive_array[2*NUM_STGAP_DEVICES + i] >> 3 & 1);
        stgap_faults.dt_err[i] = (bool)(receive_array[2*NUM_STGAP_DEVICES + i] >> 4 & 1);
    }
    return &stgap_faults;
}

uint8_t Io_STGAP1AS_SendReceiveByte(
        uint8_t        byte_value,
    const uint8_t        crc_xor,
    uint8_t *const receive_array)
{
    uint8_t receive_buffer[NUM_STGAP_DEVICES * 2];
    uint8_t byte_crc;

    byte_crc = Io_STGAP1AS_CalculateCrcTx(byte_value, crc_xor);

    // Send byte1 and corresponding CRC value to all devices
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES * 2; i = i + 2)
    {
        // Pull chip select line low
        HAL_GPIO_WritePin(
            GDRV_SPI_CS_GPIO_Port, GDRV_SPI_CS_Pin, GPIO_PIN_RESET);
        // Transmit byte1 value, receive nonsense
        HAL_SPI_TransmitReceive(
            stgap_spi_handle, &byte_value, &receive_buffer[i], 1,
            STGAP_SPI_TIMEOUT);
        // Transmit CRC for byte1 value, receive nonsense
        HAL_SPI_TransmitReceive(
            stgap_spi_handle, &byte_crc, &receive_buffer[i + 1], 1,
            STGAP_SPI_TIMEOUT);
        HAL_GPIO_WritePin(GDRV_SPI_CS_GPIO_Port, GDRV_SPI_CS_Pin, GPIO_PIN_SET);
    }

    // Verify CRC on all received bytes
    for (uint8_t i = 0; i < (NUM_STGAP_DEVICES * 2); i = i + 2)
    {
        if (receive_buffer[i + 1] !=
            Io_STGAP1AS_CalculateCrcRx(receive_buffer[i], 0xFF))
        {
            // FAULT HANDLING HERE (if crc doesn't match)
        }
    }

    // Populate receive array
    for (uint8_t i = 0; i < NUM_STGAP_DEVICES; i++)
    {
        *(receive_array + i) = receive_buffer[i * 2];
    }
    return byte_crc;
}

uint8_t Io_STGAP1AS_CalculateCrcTx(const uint8_t input, const uint8_t xor_value)
{
    uint8_t xor_input  = input ^ ~xor_value;
    uint8_t crc        = crc_table[xor_input];
    uint8_t xor_output = crc ^ 0xFF;
    return xor_output;
}

uint8_t Io_STGAP1AS_CalculateCrcRx(const uint8_t input, const uint8_t xor_value)
{
    uint8_t xor_input = input ^ ~xor_value;
    uint8_t crc       = crc_table[xor_input];
    return crc;
}

bool Io_STGAP1AS_GetShutdownPin(void)
{
    if(HAL_GPIO_ReadPin(nSHDN_GDRV_GPIO_Port, nSHDN_GDRV_Pin))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Io_STGAP1AS_SetShutdownPin(const bool value)
{
    if (value)
    {
        HAL_GPIO_WritePin(nSHDN_GDRV_GPIO_Port, nSHDN_GDRV_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(nSHDN_GDRV_GPIO_Port, nSHDN_GDRV_Pin, GPIO_PIN_RESET);
    }
}
