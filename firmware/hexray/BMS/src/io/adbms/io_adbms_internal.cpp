#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"
#include "io_semaphore.hpp"

#include <cstring>
#include <algorithm>

using namespace std;

namespace
{
io::semaphore spi_bus_lock{ true };

consteval std::array<uint16_t, 256> pecTable(const uint16_t poly, const uint16_t size)
{
    const auto MSB_SIZE  = static_cast<uint16_t>(1 << (size - 1));
    const auto MASK_SIZE = static_cast<uint16_t>((1 << (size + 1)) - 1);

    array<uint16_t, 256> out{};
    for (uint16_t i = 0; i < 256; i++)
    {
        auto r = static_cast<uint16_t>(i << (size - 8)); // r = 4 * i
        for (int b = 7; b >= 0; --b)
        {
            if (r & MSB_SIZE)
            { // check msb is set
                r = static_cast<uint16_t>(r << 1);
                r ^= poly;
            }
            else
            {
                r = static_cast<uint16_t>(r << 1);
            }
        }
        out[i] = r & MASK_SIZE;
    }
    return out;
}

template <std::integral T> T swapEndianness(const T value)
{
    T out = 0;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        out = static_cast<T>(out << 8);
        out |= static_cast<T>(value >> (i * 8) & 0xFFU);
    }
    return out;
}
} // namespace

namespace io::adbms
{
/**
 * This represents the wire representation of a register group read from the chip, which consists of the raw bytes
 * followed by the PEC10.
 */
class __attribute__((packed)) RegGroupPayload
{
    static constexpr uint16_t             CRC10_POLY = 0x8F;
    static constexpr array<uint16_t, 256> pec10Table = pecTable(CRC10_POLY, 10);
    RegBuffer                             data;
    // when the data is in here, it is in wire order (big endian). Please make sure to swap endianness on read or write
    uint16_t pec10_cmd_count;

    /**
     * @return This will return a 10 bit number
     */
    [[nodiscard]] uint16_t calculatePec10() const
    {
        uint16_t remainder = 16U;

        for (const uint8_t i : data)
        {
            const uint16_t address = (remainder >> 2 ^ i) & 0xFFU;
            remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec10Table[address]);
            remainder &= 0x03FFU; // pec10 keeps only 10 bits
        }

        remainder ^= static_cast<uint16_t>(cmd_count() << 4);
        for (int bit = 0; bit < 6; bit++)
        {
            if (remainder & 0x0200) // check top bit
                remainder = static_cast<uint16_t>((remainder << 1U) ^ CRC10_POLY);
            else
                remainder = static_cast<uint16_t>(remainder << 1U);
            remainder &= 0x03FF; // ensure remainder stays 10 bits
        }
        return remainder;
    }

  public:
    // constructor for building reggrouppayloads when sending
    explicit RegGroupPayload(const array<uint8_t, REG_GROUP_SIZE> &_data)
      : data(_data), pec10_cmd_count(swapEndianness(calculatePec10()))
    {
    }
    // default for recieving
    RegGroupPayload() : data{}, pec10_cmd_count(0) {}

    [[nodiscard]] bool checksum() const { return calculatePec10() == pec10(); }

    [[nodiscard]] uint16_t pec10() const
    {
        // pec10 is stored in the lower 10 bits of the pec10_cmd_count field
        return static_cast<uint16_t>(swapEndianness(pec10_cmd_count) & 0x03FFU);
    }
    [[nodiscard]] uint8_t cmd_count() const
    {
        // cmd_count is stored in the upper 6 bits of the pec10_cmd_count field
        return static_cast<uint8_t>(swapEndianness(pec10_cmd_count) >> 10U);
    }
    [[nodiscard]] const RegBuffer &getData() const { return data; }
};
static_assert(sizeof(RegGroupPayload) == REG_GROUP_SIZE + 2); // 2 bytes for PEC10 and cmd_count

class __attribute__((packed)) Cmd
{
    static constexpr uint16_t             CRC15_POLY = 0x4599;
    static constexpr array<uint16_t, 256> pec15Table = pecTable(CRC15_POLY, 15);

    // note that both are stored in big endian on the wire, so swap endianness when reading and writing
    uint16_t cmd;
    uint16_t pec15;

    [[nodiscard]] uint16_t calculatePec15() const
    {
        uint16_t remainder = 16U;

        for (const uint8_t i :
             { static_cast<uint8_t>(cmd), static_cast<uint8_t>(cmd >> 8) }) // note that the endianness is swapped
        {
            const uint16_t address = ((remainder >> 7) ^ i) & 0xFFU;
            remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec15Table[address]);
        }
        return static_cast<uint16_t>(remainder << 1U);
    }

  public:
    explicit Cmd(const uint16_t _cmd)
    {
        cmd   = swapEndianness(_cmd);
        pec15 = swapEndianness(calculatePec15());
    }
    [[nodiscard]] span<const uint8_t> into_span() const
    {
        return { reinterpret_cast<const uint8_t *>(this), sizeof(Cmd) };
    }
};
static_assert(sizeof(Cmd) == 4); // 2 bytes for cmd and 2 bytes for PEC15

struct __attribute__((packed)) WriteCmd
{
    Cmd                       tx_cmd;
    Segments<RegGroupPayload> payload;

    explicit WriteCmd(const uint16_t cmd, const Segments<RegBuffer> regs) : tx_cmd(cmd)
    {
        for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
        {
            payload[segment] = RegGroupPayload{ regs[segment] };
        }
    }

    [[nodiscard]] span<uint8_t> into_span() { return { reinterpret_cast<uint8_t *>(this), sizeof(WriteCmd) }; }
};

result<void> sendCmd(const uint16_t cmd)
{
    const io::unique_semaphore lock{ spi_bus_lock };
    const Cmd                   tx_cmd{ cmd };
    return adbms_spi_ls.transmitDma(tx_cmd.into_span());
}

result<bitset<32>> poll(const uint16_t cmd)
{
    const io::unique_semaphore lock{ spi_bus_lock };
    const Cmd                   tx_cmd{ cmd };
    uint32_t                    poll_buf;
    static_assert(sizeof(poll_buf) == 4);
    const auto status = adbms_spi_ls.transmitThenReceiveDma(
        tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(&poll_buf), sizeof(poll_buf) });
    return status ? result<std::bitset<32>>{ poll_buf } : unexpected(status.error());
}

Segments<result<RegBuffer>> readRegGroup(const uint16_t cmd)
{
    const io::unique_semaphore  lock{ spi_bus_lock };
    Segments<result<RegBuffer>> regs;
    const Cmd                   tx_cmd{ cmd };
    Segments<RegGroupPayload>   rx_buffer;

    if (const auto comm_status = adbms_spi_ls.transmitThenReceiveDma(
            tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(rx_buffer.data()), sizeof(rx_buffer) });
        !comm_status)
    {
        regs.fill(std::unexpected(comm_status.error()));
        return regs;
    }

    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        const auto &segment_reg_group = rx_buffer[segment];
        if (not segment_reg_group.checksum())
        {
            regs[segment] = unexpected(ErrorCode::CHECKSUM_FAIL);
            continue;
        }
        regs[segment] = segment_reg_group.getData();
    }

    return regs;
}

result<void> writeRegGroup(const uint16_t cmd, Segments<RegBuffer> &regs)
{
    const io::unique_semaphore lock{ spi_bus_lock };
    ranges::reverse(regs);
    WriteCmd tx_buffer(cmd, regs);
    return adbms_spi_ls.transmitDma(tx_buffer.into_span());
}

} // namespace io::adbms
