#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "hw_spi.hpp"
#include "util_errorCodes.hpp"

namespace {

    // Resets to 0 on powerup/RSTCC and rolls over from 63 to 1
    uint8_t cmd_count = 0U;

    consteval std::array<uint16_t, 256> generatePecTable(const uint16_t poly, const uint16_t size) {
        const uint16_t msb_mask = static_cast<uint16_t>(1U << (size - 1));
        std::array<uint16_t, 256> table{};

        for (uint16_t byte = 0; byte < 256; ++byte) {
            uint16_t remainder = static_cast<uint16_t>(byte << (size - 8));
            for (int bit = 0; bit < 8; ++bit) {
                const bool msb = (remainder & msb_mask) != 0;
                remainder = static_cast<uint16_t>(remainder << 1);
                if (msb) remainder ^= poly;
            }
            table[byte] = remainder;
        }
        return table;
    }

    class __attribute__((packed)) CmdPayload {
        private: 
            static constexpr uint16_t             PEC15_POLY = 0x4599;
            static constexpr uint16_t             PEC15_SIZE = 15;
            static constexpr std::array<uint16_t, 256> pec15Table = generatePecTable(PEC15_POLY, PEC15_SIZE);

            uint16_t cmd;
            uint16_t pec15;

            [[nodiscard]] uint16_t calculatePec15() const {
                uint16_t remainder = 16U;
                for (const uint8_t byte : { static_cast<uint8_t>(cmd & 0xFFU), static_cast<uint8_t>(cmd >> 8) }) {
                    const uint16_t address = ((remainder >> 7) ^ byte) & 0xFFU;
                    remainder = static_cast<uint16_t>((remainder << 8) ^ pec15Table[address]);
                }
                return static_cast<uint16_t>(remainder << 1);
            }

        public: 
            // MCU is little endian but ADBMS expects most significant byte first, so swap before sending
            explicit CmdPayload(const uint16_t _cmd) {
                cmd = std::byteswap(_cmd);
                pec15 = std::byteswap(calculatePec15());
            }
    };
    static_assert(sizeof(CmdPayload) == 4);

    class __attribute__((packed)) DataPayload {
        protected:
            static constexpr uint16_t           PEC10_POLY = 0x008F;
            static constexpr uint16_t           PEC10_SIZE = 10;
            static constexpr std::array<uint16_t, 256> pec10Table = generatePecTable(PEC10_POLY, PEC10_SIZE);

            io::adbms::RegGroup data;
            uint16_t pec10;

            // Calculates PEC10 based on 6 data bytes
            [[nodiscard]] uint16_t calculatePec10() const {
                uint16_t remainder = 16U;
                for (const uint8_t byte : data) {
                    const uint16_t address = ((remainder >> 2) ^ byte) & 0xFFU;
                    remainder = static_cast<uint16_t>((remainder << 8) ^ pec10Table[address]);
                }
                return static_cast<uint16_t>(remainder & 0x3FFU);
            }
    };

    class TxDataPayload : public DataPayload {
        public:
            TxDataPayload() = default;
            // MCU is little endian but ADBMS expects most significant byte first, so swap before sending
            explicit TxDataPayload(const io::adbms::RegGroup _data) {
                data = _data;
                pec10 = std::byteswap(calculatePec10());
            }
    };
    static_assert(sizeof(TxDataPayload) == 8);
    

    class RxDataPayload : public DataPayload {
        public:
            [[nodiscard]] bool checksum() const {
                const uint16_t rx_pec = std::byteswap(pec10);

                // Calculates PEC10 based on 6 data bytes AND command counter
                uint16_t remainder = static_cast<uint16_t>(calculatePec10() ^ ((rx_pec >> 10) << 4));
                for (int bit = 0; bit < 6; ++bit) {
                    const bool msb = (remainder & 0x200U) != 0;
                    remainder = static_cast<uint16_t>(remainder << 1);
                    if (msb) remainder ^= PEC10_POLY;
                }
                return (remainder & 0x3FFU) == (rx_pec & 0x3FFU);
            }

            [[nodiscard]] uint8_t cmdCount() const {
                return static_cast<uint8_t>(std::byteswap(pec10) >> 10);
            }

            [[nodiscard]] const io::adbms::RegGroup &getData() const {
                return data;
            }
    };
    static_assert(sizeof(RxDataPayload) == 8);
}


namespace io::adbms::spi {
    result<void> sendCmd(const hw::spi::device &port, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        RETURN_IF_ERR_SILENT(port.transmitDma({reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(CmdPayload)}));
        cmd_count = cmd == RSTCC ? 0U : static_cast<uint8_t>(cmd_count % 63U + 1U);
        return {};
    }

    
    result<uint32_t> poll(const hw::spi::device &port, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        uint32_t poll_buf; // Polling data is only valid after 2 * NUM_SEGMENTS bits
        const result<void> status = port.transmitThenReceiveDma({reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(CmdPayload)}, { reinterpret_cast<uint8_t *>(&poll_buf), sizeof(poll_buf) });
        return status ? result<uint32_t>{ poll_buf } : std::unexpected(status.error());
    } 

    result<void> readRegGroup(const hw::spi::device &port, const uint16_t cmd, const std::span<result<RegGroup>> rx) {
        // Span size may change according to isoSPI breaks but must be at most NUM_SEGMENTS
        if (rx.size() > NUM_SEGMENTS) return std::unexpected(ErrorCode::INVALID_ARGS); 

        const CmdPayload tx_cmd {cmd};
        Segments<RxDataPayload> rx_buf;

        RETURN_IF_ERR_SILENT(port.transmitThenReceiveDma({reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(CmdPayload)}, { reinterpret_cast<uint8_t *>(rx_buf.data()), rx.size() * sizeof(RxDataPayload) }));

        // Only need to check upto span size, rest of data is ignored
        for (size_t segment = 0U; segment < rx.size(); ++segment) { 
            
            // Checksum error indicates break in chain or invalid data; needs to be reported on RegGroup/segment level
            if (!rx_buf[segment].checksum()) rx[segment] = std::unexpected(ErrorCode::CHECKSUM_FAIL);
            // Command count mismatch means that segment missed a command
            else if (rx_buf[segment].cmdCount() != cmd_count) rx[segment] = std::unexpected(ErrorCode::CMD_COUNT_MISMATCH);
            else rx[segment] = rx_buf[segment].getData();
        }
        return {};
    }


    result<void> writeRegGroup(const hw::spi::device &port, const uint16_t cmd, const std::span<const RegGroup> tx) {
        // Span size may change according to isoSPI breaks but must be at most NUM_SEGMENTS
        if (tx.size() > NUM_SEGMENTS) return std::unexpected(ErrorCode::INVALID_ARGS);

        struct {
            CmdPayload            cmd;
            Segments<TxDataPayload> data;
        } tx_buf { CmdPayload{cmd}, {} };

        // First payload is sent to farthest device so send in reverse order
        for (size_t segment = 0U; segment < tx.size(); ++segment) {
            tx_buf.data[segment] = TxDataPayload{ tx[tx.size() - 1U - segment] };
        }

        RETURN_IF_ERR_SILENT(port.transmitDma({ reinterpret_cast<const uint8_t *>(&tx_buf), sizeof(CmdPayload) + tx.size() * sizeof(TxDataPayload) }));
        cmd_count = static_cast<uint8_t>(cmd_count % 63U + 1U);
        return {};
    }


}
