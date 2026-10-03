#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "hw_spis.hpp"

#include <bit>

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

            [[nodiscard]] span<const uint8_t> into_span() const {
                return {static_cast<const uint8_t *>(this), sizeof(CmdPayload)};
            }
    }

    class __attribute__((packed)) DataPayload {
        private:
            static constexpr uint16_t           PEC10_POLY = 0x008F;
            static constexpr uint16_t           PEC10_SIZE = 10;
            static constexpr std::array<uint16_t, 256> pec10Table = generatePecTable(PEC10_POLY, PEC10_SIZE);

            RegGroup data;
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
            explicit DataPayload(const RegGroup _data) {
                data = _data;
                pec10 = std::byteswap(calculatePec10());
            }

            [[nodiscard]] span<const uint8_t> into_span() const {
                return {static_cast<const uint8_t *> (this), sizeof(DataPayload)};
            }

    }

    class __attribute__((packed)) TxDataPayload: DataPayload {
            
    }

    class __attribute__((packed)) RxDataPayload: DataPayload {

    }


}

namespace io::adbms::spi {
    result<void> sendCmd(const hw::spi::device &port, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        return port.transmitDma(tx_cmd.into_span());
    }

    result<void> poll(const hw::spi::device &port, const size_t num_segments, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        uint

        return port.transmitThenReceiveDma(tx_cmd.into_span(), )

    } 

    template <typename T>
    std::array<result<void>, T> readRegGroup(const hw::spi::device &port, const size_t num_segments, const uint16_t cmd) {
        const CmdPayload tx_cmd {cmd};
        const DataPayload rx_data {}
    }

    template <typename T>
    std::array<result<void>, T> writeRegGroup(const hw::spi::device &port, const size_t num_segments, const uint16_t cmd, const RegGroup tx) {
        const CmdPayload tx_cmd {cmd};
        const DataPayload tx_data {tx}
        return port.transmitDma(tx_data.into)
        
    } 

}
