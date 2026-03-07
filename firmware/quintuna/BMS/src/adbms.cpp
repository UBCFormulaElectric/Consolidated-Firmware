#include "adbms.h"
#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "app_segments.hpp"
#include "util_errorCodes.hpp"
#include "io_time.hpp"
#include "stm32h7xx_hal.h"
#include "main.h"

#include <cassert>
#include <array>
#include <cstring>

static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configs;
static std::array<std::expected<void, ErrorCode>, io::NUM_SEGMENTS> success;
static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configReg;

static std::array<std::array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_regs;
static std::array<std::array<std::expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> comm_success;

CFUNC void adbms_init()
{
    for (auto &[reg_a, reg_b] : configReg)
    {
        reg_a.cth       = 1;
        reg_a.ref_on    = 1;
        reg_a.flag_d    = 0;
        reg_a.owa       = 0;
        reg_a.owrng     = 0;
        reg_a.soak_on   = 0;
        reg_a.gpio_1_8  = 0b11111111;
        reg_a.gpio_9_10 = 0b11;
        reg_a.fc        = 0;
        reg_a.comm_bk   = 0;
        reg_a.mute_st   = 0;
        reg_a.snap_st   = 0;

        constexpr uint16_t vuv = 0b110100001;
        constexpr uint16_t vov = 0b10001100101;
        reg_b.vuv_0_7          = static_cast<uint8_t>(vuv & 0xFF);
        reg_b.vuv_8_11         = static_cast<uint8_t>(vuv >> 8 & 0x0F);
        reg_b.vov_0_3          = static_cast<uint8_t>(vov & 0x0F);
        reg_b.vov_4_11         = static_cast<uint8_t>(vov >> 4 & 0xFF);
        reg_b.dcto_0_5         = 0;
        reg_b.dtrng            = 0;
        reg_b.dtmen            = 0;
        reg_b.dcc_1_8          = 0xFF;
        reg_b.dcc_9_16         = 0xFF;
    }

    assert(io::adbms::wakeup());
    assert(io::adbms::writeConfigReg(configReg));
}

// ------- Jack's DMA ConfigReg validation -------

// ISR handled in shared/hw_spi.cpp

// Packet layout matches Pranay's writeRegGroup/readRegGroup exactly
struct __attribute__((packed)) CfgPacket
{
    io::adbms::TxCmd                                         tx_cmd;
    std::array<io::adbms::RegGroupPayload, io::NUM_SEGMENTS> payload;
};

__attribute__((section(".sram2_data"))) static CfgPacket cfg_tx_a;
__attribute__((section(".sram2_data"))) static CfgPacket cfg_rx_a;
__attribute__((section(".sram2_data"))) static CfgPacket cfg_tx_b;
__attribute__((section(".sram2_data"))) static CfgPacket cfg_rx_b;

// State machine for tracking where we are in the DMA process
enum class DmaCfgState
{
    IDLE,
    WRITING_A,
    WRITING_B,
    READING_A,
    READING_B,
};

static volatile DmaCfgState dma_cfg_state = DmaCfgState::IDLE;

// Parsed readback results (populates after READING_B completes)
static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> dma_cfg_readback;
static std::array<bool, io::NUM_SEGMENTS> dma_cfg_pec_ok;

// Build a write packet (same as Pranay's writeRegGroup's tx_buffer construction)
static void buildWritePacket(CfgPacket &tx, const uint16_t cmd, const std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> &cfg, bool useRegA)
{
    tx.tx_cmd = io::adbms::TxCmd{ cmd };
    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        const uint8_t *data = useRegA
            ? reinterpret_cast<const uint8_t *>(&cfg[seg].reg_a)
            : reinterpret_cast<const uint8_t *>(&cfg[seg].reg_b);
        std::memcpy(tx.payload[seg].data.data(), data, io::adbms::REG_GROUP_SIZE);
        tx.payload[seg].pec10 = io::adbms::swapEndianness(
            static_cast<uint16_t>(io::adbms::calculatePec10(tx.payload[seg].data, 0U) & 0x03FFU));
    }
}

// Build a read packet (command + 0xFF dummy payload for DMA clocking)
static void buildReadPacket(CfgPacket &tx, const uint16_t cmd)
{
    tx.tx_cmd = io::adbms::TxCmd{ cmd };
    std::memset(tx.payload.data(), 0xFF, sizeof(tx.payload));
}

// Start the write phase (CFGA first)
void writeConfigRegDma(const std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> &cfg)
{
    if (dma_cfg_state != DmaCfgState::IDLE)
        return;

    buildWritePacket(cfg_tx_a, io::adbms::WRCFGA, cfg, true);
    dma_cfg_state = DmaCfgState::WRITING_A;

    HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_TransmitReceive_DMA(&hspi4, reinterpret_cast<uint8_t *>(&cfg_tx_a), reinterpret_cast<uint8_t *>(&cfg_rx_a), sizeof(CfgPacket)) != HAL_OK)
    {
        HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
        dma_cfg_state = DmaCfgState::IDLE;
        LOG_WARN("writeConfigRegDma: WRCFGA DMA start failed");
    }
}

// Start readback (RDCFGA first) (call after writes complete)
static void startReadCfgA()
{
    buildReadPacket(cfg_tx_a, io::adbms::RDCFGA);
    dma_cfg_state = DmaCfgState::READING_A;

    HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_TransmitReceive_DMA(&hspi4, reinterpret_cast<uint8_t *>(&cfg_tx_a), reinterpret_cast<uint8_t *>(&cfg_rx_a), sizeof(CfgPacket)) != HAL_OK)
    {
        HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
        dma_cfg_state = DmaCfgState::IDLE;
        LOG_WARN("writeConfigRegDma: RDCFGA DMA start failed");
    }
}

// Called from the HAL_SPI_TxRxCpltCallback (might want to change this to run on the main loop instead of the ISR)
void onDmaCfgComplete()
{
    HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);

    switch (dma_cfg_state)
    {
        case DmaCfgState::WRITING_A:
        {
            // WRCFGA done so now write CFGB
            // Reuse cfg_tx_b; need access to cfg – stored in configReg (file-scope)
            buildWritePacket(cfg_tx_b, io::adbms::WRCFGB, configReg, false);
            dma_cfg_state = DmaCfgState::WRITING_B;

            HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_RESET);
            if (HAL_SPI_TransmitReceive_DMA(&hspi4, reinterpret_cast<uint8_t *>(&cfg_tx_b), reinterpret_cast<uint8_t *>(&cfg_rx_b), sizeof(CfgPacket)) != HAL_OK)
            {
                HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
                dma_cfg_state = DmaCfgState::IDLE;
            }
            break;
        }
        case DmaCfgState::WRITING_B:
        {
            // Both config groups written -> start readback
            startReadCfgA();
            break;
        }
        case DmaCfgState::READING_A:
        {
            // cfg_rx_a.payload[] holds the chip response – validate PEC the same way readRegGroup does.
            for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
            {
                const auto &p   = cfg_rx_a.payload[seg];
                dma_cfg_pec_ok[seg] =
                    io::adbms::calculatePec10(p.data, io::adbms::swapEndianness(p.pec10) >> 10U) ==
                    io::adbms::swapEndianness(static_cast<uint16_t>(p.pec10 & 0xFF03));
                if (dma_cfg_pec_ok[seg])
                    std::memcpy(&dma_cfg_readback[seg].reg_a, p.data.data(), io::adbms::REG_GROUP_SIZE);
            }

            // Now read CFGB
            buildReadPacket(cfg_tx_b, io::adbms::RDCFGB);
            dma_cfg_state = DmaCfgState::READING_B;

            HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_RESET);
            if (HAL_SPI_TransmitReceive_DMA(&hspi4, reinterpret_cast<uint8_t *>(&cfg_tx_b), reinterpret_cast<uint8_t *>(&cfg_rx_b), sizeof(CfgPacket)) != HAL_OK)
            {
                HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
                dma_cfg_state = DmaCfgState::IDLE;
            }
            break;
        }
        case DmaCfgState::READING_B:
        {
            for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
            {
                const auto &p = cfg_rx_b.payload[seg];
                const bool  ok =
                    io::adbms::calculatePec10(p.data, io::adbms::swapEndianness(p.pec10) >> 10U) ==
                    io::adbms::swapEndianness(static_cast<uint16_t>(p.pec10 & 0xFF03));
                if (dma_cfg_pec_ok[seg] && ok)
                    std::memcpy(&dma_cfg_readback[seg].reg_b, p.data.data(), io::adbms::REG_GROUP_SIZE);
                else
                    dma_cfg_pec_ok[seg] = false;
            }

            // All done – log result for each segment
            for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
            {
                if (dma_cfg_pec_ok[seg])
                    LOG_INFO("DMA ConfigReg readback OK for segment %u", (unsigned)seg);
                else
                    LOG_WARN("DMA ConfigReg readback PEC FAIL for segment %u", (unsigned)seg);
            }

            dma_cfg_state = DmaCfgState::IDLE;
            break;
        }
        case DmaCfgState::IDLE:
        {
            break;
        }
        default:
            dma_cfg_state = DmaCfgState::IDLE;
            break;
    }
}

// Returns true when a DMA ConfigReg cycle (write + readback) is running
bool isDmaCfgBusy()
{
    return dma_cfg_state != DmaCfgState::IDLE;
}

// ------- end DMA ConfigReg validation -------

CFUNC void adbms_tick()
{
    LOG_INFO("tick!");
    assert(io::adbms::wakeup()); // Why does this run every cycle again?

    // State machine: WRITING_A \to WRITING_B \to READING_A \to READING_B \to IDLE
    // Results and per segment PEC status written to dma_cfg_readback / dma_cfg_pec_ok
    if (!isDmaCfgBusy())
    {
        writeConfigRegDma(configReg);
    }

    LOG_INFO("done!");
}
