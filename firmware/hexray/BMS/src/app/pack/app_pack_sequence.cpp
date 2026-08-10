#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "io_time.hpp"

namespace {
    uint32_t cycle = 0;
    app::pack::Snapshot local{};

    void stepCells() {

    }

    void stepTherms(ThermMux mux) {

    }

    void stepCellOpenWire(OpenWire parity) {

    }

    void stepConfig() {

    }

    void stepADBMS6830Flags() {

    }
}

namespace app::pack::sequence {
    void init() {
        cycle = 0;
        local = Snapshot{};
    }

    void tick() {
        uint32_t timeStamp = io::time::getCurrentMs();
        
        io::adbms::command::snap();
        // For ADBMS2950
        // if (runsOn(Step::TractiveCurrent, cycle))   stepTractiveCurrent();
        // if (runsOn(Step::TractiveVoltage, cycle))   stepTractiveVoltage();
        if (runsOn(Step::Cells, cycle))             stepCells();
        if (runsOn(Step::ThermsMux1, cycle))        stepTherms(ThermMux::MUX_0_7);
        if (runsOn(Step::ThermsMux2, cycle))        stepTherms(ThermMux::MUX_8_13);
        io::adbms::command::unsnap();

        if (runsOn(Step::CellOpenWireOdd, cycle))   stepCellOpenWire(OpenWire::ODD);
        if (runsOn(Step::CellOpenWireEven, cycle))  stepCellOpenWire(OpenWire::EVEN);
        if (runsOn(Step::Config, cycle))            stepConfig();
        if (runsOn(Step::ADBMS6830Flags, cycle))    stepADBMS6830Flags(); //maybe change name
        
       
        cycle++;
    }
}