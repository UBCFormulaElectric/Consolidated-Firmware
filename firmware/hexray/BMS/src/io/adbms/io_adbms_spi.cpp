#include "io_adbms.hpp"
#include "hw_spi.hpp"
#include "util_errorCodes.hpp"


namespace {

    consteval std::array<uint16_t, 256> generatePecTable(const uint16_t poly, const uint16_t size) {
        const uint16_t MSB_SIZE  = static_cast<uint16_t>(1U << (size - 1));
        const uint16_t MASK_SIZE = static_cast<uint16_t>((1U << (size + 1)) - 1);
        std::array<uint16_t, 256> out{};
    
        for (uint16_t i = 0; i < 256; i++) {
            uint16_t r = static_cast<uint16_t>(i << (size - 8));
            for (int b = 7; b >= 0; --b)    {
                bool msb = (r & MSB_SIZE) != 0; 
                r = static_cast<uint16_t>(r << 1); 
                if (msb) r ^= poly; 
        }
        out[i] = r & MASK_SIZE;
    }
    
    return out;
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
                for (const uint8_t i : { static_cast<uint8_t>(cmd & 0xFFU), static_cast<uint8_t>(cmd >> 8) }) {
                    const uint16_t address = ((remainder >> 7) ^ i) & 0xFFU;
                    remainder = static_cast<uint16_t>((remainder << 8) ^ pec15Table[address]);
                }
                return static_cast<uint16_t>(remainder << 1U);
            }

        public: 
            explicit CmdPayload(const uint16_t _cmd) {
                cmd = std::byteswap(_cmd);
                pec15 = std::byteswap(calculatePec15());
            }

            [[nodiscard]] std::span<const uint8_t> into_span() const {
                return {reinterpret_cast<const uint8_t *>(this), sizeof(CmdPayload)};
            }
    };
    static_assert(sizeof(CmdPayload) == io::adbms::CMD_SIZE + io::adbms::PEC_SIZE);


    class __attribute__((packed)) DataPayload {
        private:
            static constexpr uint16_t           PEC10_POLY = 0x008F;
            static constexpr uint16_t           PEC10_SIZE = 10;
            static constexpr std::array<uint16_t, 256> pec10Table = generatePecTable(PEC10_POLY, PEC10_SIZE);

            io::adbms::RegGroup data;
            uint16_t pec10;

            [[nodiscard]] uint16_t calculatePec10() const {
                uint16_t remainder = 16U;
                for (const uint8_t i : data) {
                    const uint16_t address = ((remainder >> 2) ^ i) & 0xFFU;
                    remainder = static_cast<uint16_t>(((remainder << 8) ^ pec10Table[address]) & 0x3FF);
                }
                return remainder;
            }

        public:
            DataPayload() = default;

            explicit DataPayload(const io::adbms::RegGroup _data) {
                data = _data;
                pec10 = std::byteswap(calculatePec10());
            }
            
            [[nodiscard]] const io::adbms::RegGroup &getData() const { 
                return data; 
            }
    };
}
static_assert(sizeof(DataPayload) == io::adbms::DATA_SIZE + io::adbms::PEC_SIZE);

namespace io::adbms::spi {
    result<void> sendCmd(const hw::spi::device &port, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        return port.transmitDma(tx_cmd.into_span());
    }

    
    result<uint32_t> poll(const hw::spi::device &port, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        uint32_t poll_buf; // Polling data is only valid after 2 * NUM_SEGMENT bits 
        const result<void> status = port.transmitThenReceiveDma(tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(&poll_buf), sizeof(poll_buf) });
        return status ? result<uint32_t>{ poll_buf } : std::unexpected(status.error());
    } 

    result<void> readRegGroup(const hw::spi::device &port, const uint16_t cmd, const std::span<result<RegGroup>> rx) {
        // Span size may change according to isoSPI breaks but must be at most NUM_SEGMENTS
        if (rx.size() > NUM_SEGMENTS) return std::unexpected(ErrorCode::INVALID_ARGS); 

        const CmdPayload tx_cmd {cmd};
        Segments<DataPayload> rx_buf;

        RETURN_IF_ERR_SILENT(port.transmitThenReceiveDma(tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(rx_buf.data()), rx.size() * sizeof(DataPayload) }));

        // Only need to check upto span size, rest of data is ignored
        for (size_t segment = 0U; segment < rx.size(); ++segment) { 
            
            // Checksum error indicates break in chain or invalid data; needs to be reported on RegGroup/segment level
            rx[segment] = rx_buf[segment].checksum() ? result<RegGroup>{ rx_buf[segment].getData() } : std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }
        return {};
    }


    result<void> writeRegGroup(const hw::spi::device &port, const uint16_t cmd, const std::span<const RegGroup> tx) {
        // Span size may change according to isoSPI breaks but must be at most NUM_SEGMENTS
        if (tx.size() > NUM_SEGMENTS) return std::unexpected(ErrorCode::INVALID_ARGS);

        struct {
            CmdPayload            cmd;
            Segments<DataPayload> data;
        } tx_buf { CmdPayload{cmd}, {} };
        
        // First payload is sent to farthest device so send in reverse order
        for (size_t segment = 0U; segment < tx.size(); ++segment) {
            tx_buf.data[segment] = DataPayload{ tx[tx.size() - 1U - segment] };
        }

        return port.transmitDma({ reinterpret_cast<const uint8_t *>(&tx_buf), sizeof(CmdPayload) + tx.size() * sizeof(DataPayload) });
    }


}
