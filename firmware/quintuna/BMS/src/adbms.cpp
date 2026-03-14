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

static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS>       configReg;

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

// One command frame: command+PEC followed by one payload per segment.
struct __attribute__((packed)) CfgPacket
{
    io::adbms::TxCmd                                         tx_cmd;
    std::array<io::adbms::RegGroupPayload, io::NUM_SEGMENTS> payload;
};

// Combined frame for CFGA then CFGB.
struct __attribute__((packed)) CfgDualPacket
{
    CfgPacket reg_a;
    CfgPacket reg_b;
};

// Two reusable DMA buffers in SRAM2 and cache-line aligned for Cortex-M7 DCache maintenance.
__attribute__((section(".sram2_data"), aligned(32))) static CfgDualPacket cfg_tx_dma;
__attribute__((section(".sram2_data"), aligned(32))) static CfgDualPacket cfg_rx_dma;

// State machine for tracking where we are in the DMA process
enum class DmaCfgState
{
    IDLE,
    WRITING,
    READING,
};

static volatile DmaCfgState dma_cfg_state = DmaCfgState::IDLE;

// Flags for ISR to set
static volatile bool dma_cfg_irq_complete = false;
static volatile bool dma_cfg_irq_error    = false;

// Parsed readback results (populates after combined read of both reg groups)
static std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> dma_cfg_readback;
static std::array<bool, io::NUM_SEGMENTS> dma_cfg_pec_ok;

static void clearDmaIrqFlags()
{
    dma_cfg_irq_complete = false;
    dma_cfg_irq_error    = false;
}

static bool startDmaTransfer(const DmaCfgState nextState, const char *failLog)
{
    clearDmaIrqFlags();
    dma_cfg_state = nextState;

    HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_RESET);
    SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t *>(&cfg_tx_dma), sizeof(CfgDualPacket));

    if (HAL_SPI_TransmitReceive_DMA(
            &hspi4,
            reinterpret_cast<uint8_t *>(&cfg_tx_dma),
            reinterpret_cast<uint8_t *>(&cfg_rx_dma),
            sizeof(CfgDualPacket)) != HAL_OK)
    {
        HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
        dma_cfg_state = DmaCfgState::IDLE;
        LOG_WARN("%s", failLog);
        return false;
    }

    return true;
}

// Build one write command frame.
static void buildWritePacket(
    CfgPacket &tx,
    const uint16_t cmd,
    const std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> &cfg,
    bool useRegA)
{
    // Properly format command and command PEC.
    tx.tx_cmd.cmd = io::adbms::swapEndianness(cmd);
    uint8_t cmd_bytes[2] = { static_cast<uint8_t>(cmd >> 8), static_cast<uint8_t>(cmd & 0xFF) };
    tx.tx_cmd.pec15 = io::adbms::swapEndianness(io::adbms::calculatePec15(cmd_bytes));

    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        // Reverse segment order for daisy-chain transmission.
        uint8_t target_idx = io::NUM_SEGMENTS - 1 - seg;

        const uint8_t *data = useRegA
            ? reinterpret_cast<const uint8_t *>(&cfg[seg].reg_a)
            : reinterpret_cast<const uint8_t *>(&cfg[seg].reg_b);
        
        std::memcpy(tx.payload[target_idx].data.data(), data, io::adbms::REG_GROUP_SIZE);
        tx.payload[target_idx].pec10 = io::adbms::swapEndianness(
            static_cast<uint16_t>(io::adbms::calculatePec10(tx.payload[target_idx].data, 0U) & 0x03FFU));
    }
}

// Build one read command frame with dummy payload bytes.
static void buildReadPacket(CfgPacket &tx, const uint16_t cmd)
{
    // Properly format command and command PEC.
    tx.tx_cmd.cmd = io::adbms::swapEndianness(cmd);
    uint8_t cmd_bytes[2] = { static_cast<uint8_t>(cmd >> 8), static_cast<uint8_t>(cmd & 0xFF) };
    tx.tx_cmd.pec15 = io::adbms::swapEndianness(io::adbms::calculatePec15(cmd_bytes));

    std::memset(tx.payload.data(), 0xFF, sizeof(tx.payload));
}

static bool startWriteDma(const std::array<io::adbms::SegmentConfig, io::NUM_SEGMENTS> &cfg)
{
    if (dma_cfg_state != DmaCfgState::IDLE)
    {
        return false;
    }

    buildWritePacket(cfg_tx_dma.reg_a, io::adbms::WRCFGA, cfg, true);
    buildWritePacket(cfg_tx_dma.reg_b, io::adbms::WRCFGB, cfg, false);

    return startDmaTransfer(DmaCfgState::WRITING, "adbms DMA write failed");
}

static bool startReadDma()
{
    buildReadPacket(cfg_tx_dma.reg_a, io::adbms::RDCFGA);
    buildReadPacket(cfg_tx_dma.reg_b, io::adbms::RDCFGB);

    return startDmaTransfer(DmaCfgState::READING, "adbms DMA read start failed");
}

static void processCombinedReadback()
{
    SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t*>(&cfg_rx_dma), sizeof(CfgDualPacket));

    for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        const auto &p_a = cfg_rx_dma.reg_a.payload[seg];
        const auto &p_b = cfg_rx_dma.reg_b.payload[seg];

        const bool ok_a =
            io::adbms::calculatePec10(p_a.data, io::adbms::swapEndianness(p_a.pec10) >> 10U) ==
            io::adbms::swapEndianness(static_cast<uint16_t>(p_a.pec10 & 0xFF03));
        const bool ok_b =
            io::adbms::calculatePec10(p_b.data, io::adbms::swapEndianness(p_b.pec10) >> 10U) ==
            io::adbms::swapEndianness(static_cast<uint16_t>(p_b.pec10 & 0xFF03));

        dma_cfg_pec_ok[seg] = ok_a && ok_b;
        if (dma_cfg_pec_ok[seg])
        {
            std::memcpy(&dma_cfg_readback[seg].reg_a, p_a.data.data(), io::adbms::REG_GROUP_SIZE);
            std::memcpy(&dma_cfg_readback[seg].reg_b, p_b.data.data(), io::adbms::REG_GROUP_SIZE);
            LOG_INFO("DMA ConfigReg readback OK for segment %u", static_cast<unsigned>(seg));
        }
        else
        {
            LOG_WARN("DMA ConfigReg readback PEC FAIL for segment %u", static_cast<unsigned>(seg));
        }
    }
}

// Called from SPI HAL callbacks. ISR work is intentionally minimal.
void onDmaCfgComplete()
{
    dma_cfg_irq_complete = true;
}

void onDmaCfgError()
{
    dma_cfg_irq_error = true;
}

static void processDmaCfgStateMachine()
{
    if (dma_cfg_irq_error)
    {
        dma_cfg_irq_error    = false;
        dma_cfg_irq_complete = false;
        HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
        dma_cfg_state = DmaCfgState::IDLE;
        LOG_WARN("adbms DMA error during config transaction");
        return;
    }

    switch (dma_cfg_state)
    {
        case DmaCfgState::IDLE:
            break;

        case DmaCfgState::WRITING:
            if (!dma_cfg_irq_complete)
            {
                break;
            }

            dma_cfg_irq_complete = false;
            HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
            (void)startReadDma();
            break;

        case DmaCfgState::READING:
            if (!dma_cfg_irq_complete)
            {
                break;
            }

            dma_cfg_irq_complete = false;
            HAL_GPIO_WritePin(SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin, GPIO_PIN_SET);
            processCombinedReadback();
            dma_cfg_state = DmaCfgState::IDLE;
            break;

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
    LOG_INFO("TICK!");
    assert(io::adbms::wakeup());

    processDmaCfgStateMachine();

    if (!isDmaCfgBusy())
    {
        (void)startWriteDma(configReg);
    }

    LOG_INFO("DONE!");
}
