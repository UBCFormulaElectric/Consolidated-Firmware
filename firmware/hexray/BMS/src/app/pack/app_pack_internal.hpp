#pragma once

#include "io_adbms.hpp"

enum class Step {
    Cells,
    ThermsMux1,
    ThermsMux2,
    CellOpenWireOdd,
    CellOpenWireEven,
    Config,
    ADBMS6830Flags
};