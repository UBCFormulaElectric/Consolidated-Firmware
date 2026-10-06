#include "io_adbms_internal.hpp"
#include "app_pack.hpp"

#include <limits>

namespace {

    constexpr float   ADC_LSB_V        = 150e-6f;
    constexpr float   ADC_OFFSET_V     = 1.5f;
    constexpr int16_t ADC_CLEARED_CODE = INT16_MIN; // 0x8000, held until a conversion lands in the register

    constexpr std::array<uint16_t, 5> RDCV  { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE };
    constexpr std::array<uint16_t, 5> RDSV  { RDSVA, RDSVB, RDSVC, RDSVD, RDSVE };
    constexpr std::array<uint16_t, 5> RDAUX { RDAUXA, RDAUXB, RDAUXC, RDAUXD, RDAUXC };

    constexpr size_t                  CELLS_PER_GROUP = REG_GROUP_SIZE / sizeof(int16_t);
    constexpr size_t                  THERMS_PER_GROUP = REG_GROUP_SIZE / sizeof(int16_t);

}

namespace io::adbms::read {
    Segments<result<SegmentConfig>> configReg() {
        Segments<result<SegmentConfig>> out;
        out.fill(std::unexpected(ErrorCode::UNIMPLEMENTED));
        return out;
    }

    Segments<result<PWMConfig>> pwmReg() {
        Segments<result<PWMConfig>> out;
        out.fill(std::unexpected(ErrorCode::UNIMPLEMENTED));
        return out;
    }

    result<void> Cadc(const bool redundant, app::pack::VoltStats &stats) {
        stats.valid = {};
        std::array<Segments<RegGroup>, RDCV.size()> cell_regs;
        Segments<RegGroup> stat_c{};

        for (size_t group = 0; group < RDCV.size(); group++) {
            RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDCV[group], cell_regs[group]));
        }

        if (redundant) {
            RETURN_IF_ERR_SILENT(reversible::readRegGroup(RDSTATC, stat_c));
        }

        app::pack::LocatedValue max{ .value = std::numeric_limits<float>::lowest() };
        app::pack::LocatedValue min{ .value = std::numeric_limits<float>::max() };

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
                const RegGroup &reg  = cell_regs[cell / CELLS_PER_GROUP][seg];

                const size_t lsb = reg
               

                //if (code == ADC_CLEARED_CODE || (cs_flt >> cell & 1) != 0) continue;

                const float voltage       = static_cast<float>(code) * ADC_LSB_V + ADC_OFFSET_V;
                stats.voltages[seg][cell] = voltage;
                stats.valid[seg][cell]    = true;
                if (voltage > max.value) max = { voltage, seg, cell };
                if (voltage < min.value) min = { voltage, seg, cell };
            }
        }

        if (max.value >= min.value) {
            stats.max = max;
            stats.min = min;
        }
        return {};
    }

    result<void> Sadc(const OpenWireParity parity, app::pack::OwcStats &stats) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> Xadc(const ThermistorMux mux, app::pack::TempStats &stats) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> flags(app::pack::ADBMS6830Diag &diag) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }
}