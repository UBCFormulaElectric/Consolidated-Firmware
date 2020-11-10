#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Io_LTC6813.h"
#include "Io_SharedSpi.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/Io_LTC6813Configs.h"

#define ADCOPT 0U
#define REFON 1U
#define DTEN 0U
#define VUV 0x4E1
#define VOV 0x8CA

#define MD 1U
#define DCP 0U
#define CH 0U
#define CHST 0U

static struct SharedSpi *spi_interface;

static const uint16_t crc[UINT8_MAX + 1] = {
    0x0,    0xC599, 0xCEAB, 0xB32,  0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407,
    0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951, 0xE263, 0x27FA, 0xAD97, 0x680E,
    0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B,
    0x52A2, 0x815F, 0x44C6, 0x4FF4, 0x8A6D, 0x5B2E, 0x9EB7, 0x9585, 0x501C,
    0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0, 0x6182, 0xA41B, 0x77E6,
    0xB27F, 0xB94D, 0x7CD4, 0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF,
    0xE0DD, 0x2544, 0x2BE,  0xC727, 0xCC15, 0x98C,  0xDA71, 0x1FE8, 0x14DA,
    0xD143, 0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93, 0xE5A1, 0x2038,
    0x7C2,  0xC25B, 0xC969, 0xCF0,  0xDF0D, 0x1A94, 0x11A6, 0xD43F, 0x5E52,
    0x9BCB, 0x90F9, 0x5560, 0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC,
    0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8, 0xA8EB, 0x6D72, 0x6640,
    0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE,
    0x8423, 0x41BA, 0x4A88, 0x8F11, 0x57C,  0xC0E5, 0xCBD7, 0xE4E,  0xDDB3,
    0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D,
    0xE71F, 0x2286, 0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477,
    0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26, 0x8EDB, 0x4B42, 0x4070, 0x85E9,
    0xF84,  0xCA1D, 0xC12F, 0x4B6,  0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83,
    0x3E1A, 0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E, 0xF93D, 0x3CA4,
    0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0, 0xD3A,  0xC8A3, 0xC391,
    0x608,  0xD5F5, 0x106C, 0x1B5E, 0xDEC7, 0x54AA, 0x9133, 0x9A01, 0x5F98,
    0x8C65, 0x49FC, 0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862,
    0xBDFB, 0xB6C9, 0x7350, 0x51D6, 0x944F, 0x9F7D, 0x5AE4, 0x8919, 0x4C80,
    0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5,
    0x762C, 0xFC41, 0x39D8, 0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC,
    0x846,  0xCDDF, 0xC6ED, 0x374,  0xD089, 0x1510, 0x1E22, 0xDBBB, 0xAF8,
    0xCF61, 0xC453, 0x1CA,  0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66,
    0x3054, 0xF5CD, 0x2630, 0xE3A9, 0xE89B, 0x2D02, 0xA76F, 0x62F6, 0x69C4,
    0xAC5D, 0x7fA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A,
    0x8BA7, 0x4E3E, 0x450C, 0x8095
};

void Io_LTC6813_Init(
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin)
{
    assert(spi_handle != NULL);

    spi_interface = Io_SharedSpi_Create(
        spi_handle, nss_port, nss_pin, SPI_INTERFACE_TIMEOUT_MS_LTC6813);
}

uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint32_t size)
{
    size_t pec15_lut_index;

    // Initialize the value of the PEC15 remainder to 16.
    uint16_t pec15_remainder = 16U;

    for (size_t i = 0U; i < size; i++)
    {
        pec15_lut_index = ((pec15_remainder >> 7) ^ data_buffer[i]) & 0xFF;
        pec15_remainder =
            (uint16_t)((pec15_remainder << 8) ^ crc[pec15_lut_index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(pec15_remainder << 1);
}

ExitCode Io_LTC6813_EnterReadyState(void)
{
    uint8_t rx_data;

    // Generate isoSPI traffic to wake up the daisy chain by sending a command
    // to read a single byte NUM_OF_LTC6813 times.
    for (size_t i = 0U; i < NUM_OF_CELL_MONITOR_CHIPS; i++)
    {
        if (Io_SharedSpi_Receive(spi_interface, &rx_data, 1U) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }
    }

    return EXIT_CODE_OK;
}

ExitCode Io_LTC6813_StartCellVoltageConversions(void)
{
    // The command used to start ADC conversions for battery cell voltages.
    const uint32_t ADCV = (0x260 + (MD << 7) + (DCP << 4) + CH);

    uint8_t tx_cmd[NUM_OF_CMD_BYTES];
    tx_cmd[0] = (uint8_t)(ADCV >> 8);
    tx_cmd[1] = (uint8_t)ADCV;

    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)tx_cmd_pec15;

    return (Io_SharedSpi_Transmit(spi_interface, tx_cmd, NUM_OF_CMD_BYTES) ==
            HAL_OK)
               ? EXIT_CODE_OK
               : EXIT_CODE_ERROR;
}

ExitCode Io_LTC6813_StartInternalDeviceConversions(void)
{
    // The command used to start internal device conversions.
    const uint32_t ADSTAT = (0x468 + (MD << 7) + CHST);

    uint8_t tx_cmd[NUM_OF_CMD_BYTES];
    tx_cmd[0] = (uint8_t)(ADSTAT >> 8);
    tx_cmd[1] = (uint8_t)(ADSTAT);
    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)tx_cmd_pec15;

    return (Io_SharedSpi_Transmit(spi_interface, tx_cmd, NUM_OF_CMD_BYTES) ==
            HAL_OK)
               ? EXIT_CODE_OK
               : EXIT_CODE_ERROR;
}

ExitCode Io_LTC6813_PollConversions(void)
{
    // The command used to determine the status of ADC conversions.
    const uint32_t PLADC = 0x1407;

    uint8_t tx_cmd[NUM_OF_CMD_BYTES];
    tx_cmd[0] = (uint8_t)PLADC;
    tx_cmd[1] = (uint8_t)(PLADC >> 8);

    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)tx_cmd_pec15;

    uint32_t adc_conversion_timeout_counter = 0U;
    uint8_t  rx_data;

    // If the data read back from the chip after a PLADC command is not equal to
    // 0xFF, all chips on the daisy chain have finished converting cell
    // voltages.
    do
    {
        if (Io_SharedSpi_TransmitAndReceive(
                spi_interface, tx_cmd, NUM_OF_CMD_BYTES, &rx_data, 1U) !=
            HAL_OK)
        {
            return EXIT_CODE_TIMEOUT;
        }

        ++adc_conversion_timeout_counter;

        if (adc_conversion_timeout_counter >= ADC_TIMEOUT_CYCLES_THRESHOLD)
        {
            return EXIT_CODE_TIMEOUT;
        }
    } while (rx_data == 0xFF);

    return EXIT_CODE_OK;
}

ExitCode Io_LTC6813_ConfigureRegisterA(void)
{
    // The command used to write to configuration register A.
    const uint32_t WRCFGA = 0x01;

    uint8_t tx_cmd[NUM_OF_CMD_BYTES];
    tx_cmd[0] = (uint8_t)(WRCFGA >> 8);
    tx_cmd[1] = (uint8_t)WRCFGA;
    uint16_t tx_cmd_pec15 =
        Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
    tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
    tx_cmd[3] = (uint8_t)tx_cmd_pec15;

    const uint32_t DEFAULT_CONFIG_REG[4] = {
        (REFON << 2) + (DTEN << 1) + ADCOPT, VUV,
        ((VOV & 0xF) << 4) + (VUV >> 8), (VOV >> 4)
    };

    // The payload data is 8 bytes wide. The first 6 bytes is used to configure
    // Configuration Register A, while the remaining two bytes are the PEC15 for
    // the payload data transmitted.
    uint8_t tx_payload[8] = { 0 };
    memcpy(tx_payload, DEFAULT_CONFIG_REG, 4U);
    uint16_t tx_payload_pec15 = Io_LTC6813_CalculatePec15(tx_payload, 6U);
    tx_payload[6]             = (uint8_t)(tx_payload_pec15 >> 8);
    tx_payload[7]             = (uint8_t)tx_payload_pec15;

    Io_SharedSpi_SetNssLow(spi_interface);

    // Write to Configuration Register A.
    if (Io_SharedSpi_TransmitWithoutNssToggle(
            spi_interface, tx_cmd, NUM_OF_CMD_BYTES) != HAL_OK)
    {
        Io_SharedSpi_SetNssHigh(spi_interface);
        return EXIT_CODE_ERROR;
    }

    // Transmit the payload data to all devices connected to the daisy chain.
    if (Io_SharedSpi_MultipleTransmitWithoutNssToggle(
            spi_interface, tx_payload, 8U, NUM_OF_CELL_MONITOR_CHIPS) != HAL_OK)
    {
        Io_SharedSpi_SetNssHigh(spi_interface);
        return EXIT_CODE_ERROR;
    }

    Io_SharedSpi_SetNssHigh(spi_interface);
    return EXIT_CODE_OK;
}

struct SharedSpi *Io_LTC6813_GetSpiInterface(void)
{
    return spi_interface;
}
