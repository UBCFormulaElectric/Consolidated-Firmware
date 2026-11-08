#include "io_emeter.hpp"
#include "io_emeter_internal.hpp"

#include "hw_onewire.hpp"
#include "io_log.hpp"
#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"

#include <array>
#include <cstddef>

namespace
{
/**
 * The ADBMS driver locks its SPI bus the same way. It matters more here: a reset pulse interleaved
 * into someone else's scratchpad read does not just corrupt that read, it aborts the transaction
 * for every device on the bus.
 */
io::semaphore onewire_bus_lock{ true };

/**
 * Set between startConvertAll() and endConvert(), while the strong pullup owns the bus and no
 * traffic is allowed (pg. 7). The semaphore cannot express this because the window spans a delay
 * the job layer owns, so guard it explicitly.
 */
bool conversion_active = false;
} // namespace

namespace io::emeter
{
/**
 * TODO(bring-up): placeholders. Put one sensor on the bus at a time, call readSingleRom(), and
 * paste the codes in here in loom order.
 *
 * Left zeroed on purpose: init() rejects any entry whose family code is not FAMILY_CODE, so an
 * unfilled table fails loudly at boot instead of quietly reporting twenty dead sensors.
 */
const Sensors<RomCode> sensor_rom_codes{};

namespace
{
    /**
     * reset + Skip ROM. Every device on the bus acts on whatever command follows.
     * Only safe for commands that produce no reply -- Skip ROM + Read Scratchpad would make every
     * device transmit at once and collide (pg. 11).
     */
    [[nodiscard]] result<void> addressAll()
    {
        RETURN_IF_ERR_SILENT(hw::onewire::reset());
        hw::onewire::writeByte(SKIP_ROM);
        return {};
    }

    /** reset + Match ROM + the 8 ROM bytes, leaving exactly one device listening. */
    [[nodiscard]] result<void> addressDevice(const RomCode &rom)
    {
        RETURN_IF_ERR_SILENT(hw::onewire::reset());
        hw::onewire::writeByte(MATCH_ROM);
        hw::onewire::writeBytes(rom);
        return {};
    }

    /**
     * io::semaphore wraps a plain FreeRTOS mutex, which is not recursive, so anything that needs to
     * run both standalone and as part of a larger locked sequence gets an unlocked twin. init()
     * calls this while already holding the bus.
     */
    [[nodiscard]] result<void> writeConfigAllUnlocked(const Resolution res)
    {
        RETURN_IF_ERR_SILENT(addressAll());
        hw::onewire::writeByte(WRITE_SCRATCHPAD);

        // All three bytes have to land before the next reset or the scratchpad is left corrupt
        // (pg. 11), which is why this is one writeBytes() and not three writeByte() calls with
        // room for an early return between them.
        const std::array<uint8_t, 3> payload{ { TH_UNUSED, TL_UNUSED, configByte(res) } };
        hw::onewire::writeBytes(payload);
        return {};
    }

    /** Reads and validates one device's scratchpad. */
    [[nodiscard]] result<int16_t> readOne(const RomCode &rom)
    {
        RETURN_IF_ERR_SILENT(addressDevice(rom));
        hw::onewire::writeByte(READ_SCRATCHPAD);

        // All nine bytes, even though only the first two carry temperature: the CRC covers bytes
        // 0..7, so stopping early would mean giving up the integrity check.
        Scratchpad pad{};
        hw::onewire::readBytes({ reinterpret_cast<uint8_t *>(&pad), sizeof(pad) });

        if (crc8(pad.bytes()) != 0U)
        {
            return std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }

        // The CRC alone is not enough: a bus stuck low reads as nine zero bytes, and nine zero
        // bytes are CRC-clean. Bytes 5 and 7 are fixed by the datasheet, so they catch it.
        if (pad.reserved_5 != RESERVED_BYTE_5 || pad.reserved_7 != RESERVED_BYTE_7)
        {
            return std::unexpected(ErrorCode::INVALID_READING);
        }

        const int16_t raw = pad.rawTemp();
        if (raw == POWER_ON_RESET_TEMP)
        {
            // +85 degC exactly: the device has not completed a conversion since it powered up.
            return std::unexpected(ErrorCode::INVALID_READING);
        }

        // Mask from the config byte the device just handed back rather than from our own
        // RESOLUTION, so a device whose scratchpad write was missed is decoded as what it actually
        // sampled at instead of being silently misread.
        return maskUndefinedBits(raw, resolutionOf(pad.config));
    }
} // namespace

result<void> init()
{
    const unique_semaphore lock{ onewire_bus_lock };

    // No presence pulse at all means the whole chain is dead -- open loom, no pullup, bad ground.
    // Worth failing before we blame twenty individual sensors.
    RETURN_IF_ERR(hw::onewire::reset());

    for (size_t i = 0U; i < NUM_EMETER_SENSORS; ++i)
    {
        const RomCode &rom = sensor_rom_codes[i];

        if (rom[0] != FAMILY_CODE)
        {
            LOG_ERROR(
                "emeter sensor %u: family code 0x%x is not a DS18B20 (expected 0x%x)", static_cast<unsigned>(i),
                static_cast<unsigned>(rom[0]), static_cast<unsigned>(FAMILY_CODE));
            return std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }

        // Catches a typo in the table above at boot rather than as a silent mis-address later.
        if (crc8(rom) != 0U)
        {
            LOG_ERROR("emeter sensor %u: ROM code CRC mismatch", static_cast<unsigned>(i));
            return std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }

        RETURN_IF_ERR_SILENT(addressDevice(rom));
        hw::onewire::writeByte(READ_POWER_SUPPLY);

        // pg. 12: in the read slot a parasite-powered device pulls the bus low and an externally
        // powered one leaves it high. Every device here must be parasite powered, so a high slot
        // means VDD was left floating instead of grounded -- a stuffing error we want to find on
        // the bench, not in the car.
        if (hw::onewire::readBit())
        {
            LOG_ERROR(
                "emeter sensor %u reports external VDD; it must be grounded for parasite power",
                static_cast<unsigned>(i));
            return std::unexpected(ErrorCode::ERROR);
        }
    }

    // Leave the chain configured so the first measurement cycle after init() is already valid.
    RETURN_IF_ERR(writeConfigAllUnlocked(RESOLUTION));

    LOG_INFO("emeter: %u DS18B20s present on parasite power", static_cast<unsigned>(NUM_EMETER_SENSORS));
    return {};
}

result<void> writeConfigAll(const Resolution res)
{
    const unique_semaphore lock{ onewire_bus_lock };
    return writeConfigAllUnlocked(res);
}

result<void> startConvertAll()
{
    const unique_semaphore lock{ onewire_bus_lock };

    if (conversion_active)
    {
        return std::unexpected(ErrorCode::BUSY);
    }

    RETURN_IF_ERR_SILENT(addressAll());

    // Convert T gives us 10us (max) to get the strong pullup on (pg. 7), so hw does the write and
    // the MOSFET switch in one critical section. Everything from here to endConvert() must stay
    // off the bus.
    hw::onewire::writeByteThenStrongPullup(CONVERT_T);
    conversion_active = true;
    return {};
}

void endConvert()
{
    const unique_semaphore lock{ onewire_bus_lock };
    hw::onewire::strongPullupOff();
    conversion_active = false;
}

Sensors<result<int16_t>> readRawTemps()
{
    const unique_semaphore   lock{ onewire_bus_lock };
    Sensors<result<int16_t>> temps;

    if (conversion_active)
    {
        // The pullup is still driving the bus; a reset pulse now would fight the MOSFET and
        // corrupt the in-flight conversion on every device.
        temps.fill(std::unexpected(ErrorCode::BUSY));
        return temps;
    }

    // Per sensor, so one dead device does not blind the rest of the chain -- the same shape as
    // io::adbms::read::thermGpioVoltage().
    for (size_t i = 0U; i < NUM_EMETER_SENSORS; ++i)
    {
        temps[i] = readOne(sensor_rom_codes[i]);
    }
    return temps;
}

result<RomCode> readSingleRom()
{
    const unique_semaphore lock{ onewire_bus_lock };

    RETURN_IF_ERR_SILENT(hw::onewire::reset());
    hw::onewire::writeByte(READ_ROM);

    RomCode rom{};
    hw::onewire::readBytes(rom);

    if (crc8(rom) != 0U)
    {
        // Usually means more than one device is on the bus: they all answer at once and the
        // wired-AND of their codes is garbage (pg. 11).
        LOG_ERROR("emeter: Read ROM CRC failed -- is more than one sensor on the bus?");
        return std::unexpected(ErrorCode::CHECKSUM_FAIL);
    }
    if (rom[0] != FAMILY_CODE)
    {
        LOG_ERROR("emeter: device is not a DS18B20 (family code 0x%x)", static_cast<unsigned>(rom[0]));
        return std::unexpected(ErrorCode::INVALID_READING);
    }

    LOG_INFO(
        "emeter ROM: { 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x }", static_cast<unsigned>(rom[0]),
        static_cast<unsigned>(rom[1]), static_cast<unsigned>(rom[2]), static_cast<unsigned>(rom[3]),
        static_cast<unsigned>(rom[4]), static_cast<unsigned>(rom[5]), static_cast<unsigned>(rom[6]),
        static_cast<unsigned>(rom[7]));
    return rom;
}

} // namespace io::emeter
