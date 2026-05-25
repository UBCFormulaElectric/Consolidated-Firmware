#include "io_canQueues.hpp"

io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_adbms.hpp"

namespace io::adbms::write
{
[[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &pwm_config)
{
    return result<void>{};
}

[[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &config)
{
    return result<void>{};
}
} // namespace io::adbms::write

namespace io::adbms::read
{
[[nodiscard]] Segments<result<SegmentConfig>> configReg()
{
    return Segments<result<SegmentConfig>>{};
}

[[nodiscard]] Segments<result<PWMConfig>> pwmReg()
{
    return Segments<result<PWMConfig>>{};
}

[[nodiscard]] Cells<result<uint16_t>> cellVoltage()
{
    return Cells<result<uint16_t>>{};
}

[[nodiscard]] Cells<result<uint16_t>> secondaryCellVoltage()
{
    return Cells<result<uint16_t>>{};
}

[[nodiscard]] Segments<result<uint16_t>> segVoltage()
{
    return Segments<result<uint16_t>>{};
}

[[nodiscard]] ThermGpios<result<uint16_t>> thermGpioVoltage()
{
    return ThermGpios<result<uint16_t>>{};
}

[[nodiscard]] Segments<StatusGroupsRes> status()
{
    return Segments<StatusGroupsRes>{};
}
} // namespace io::adbms::read

namespace io::adbms::command
{
[[nodiscard]] result<void> startCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> startAuxAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollSecondaryCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollCellsAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> pollAuxAdc()
{
    return result<void>{};
}

[[nodiscard]] result<void> startBalance()
{
    return result<void>{};
}

[[nodiscard]] result<void> stopBalance()
{
    return result<void>{};
}

[[nodiscard]] result<void> wakeup()
{
    return result<void>{};
}

[[nodiscard]] result<void> owcCells(OpenWireSwitch owcSwitch)
{
    return result<void>{};
}
} // namespace io::adbms::command

namespace io::adbms::clear
{
[[nodiscard]] result<void> aux()
{
    return result<void>{};
}

[[nodiscard]] result<void> cell()
{
    return result<void>{};
}

[[nodiscard]] result<void> filteredCell()
{
    return result<void>{};
}

[[nodiscard]] result<void> stat()
{
    return result<void>{};
}
} // namespace io::adbms::clear
