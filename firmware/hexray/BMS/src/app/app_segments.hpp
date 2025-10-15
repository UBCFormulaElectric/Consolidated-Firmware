#pragma once
#include <cstdint>
#include <array>

struct CellVoltage;
struct CellTemperature;

namespace app_segments {

class ADBMS {
public:
    static constexpr uint8_t NUM_SEGMENTS = 5;
    static constexpr uint8_t CELLS_PER_SEGMENT = 14;
    enum ThermistorMux
    {
        THERMISTOR_MUX_0_7,
        THERMISTOR_MUX_8_13,
        THERMISTOR_MUX_SELECT,
    };

    static constexpr float MAX_CELL_VOLTAGE_WARNING_V = 4.17f;
    static constexpr float MIN_CELL_VOLTAGE_WARNING_V = 2.7f;
    static constexpr float MAX_CELL_TEMP_WARNING_V = 59.0f;

    static constexpr uint32_t OVER_VOLTAGE_DEBOUNCE_WARNING_MS = 1000;
    static constexpr uint32_t UNDER_VOLTAGE_DEBOUNCE_WARNING_MS = 1000;
    static constexpr uint32_t OVER_TEMP_DEBOUNCE_WARNING_MS = 1000;
    static constexpr uint32_t COMM_ERR_DEBOUNCE_WARNING_MS = 1000;

    static constexpr float MAX_CELL_VOLTAGE_FAULT_V = 4.2f;
    static constexpr float MIN_CELL_VOLTAGE_FAULT_V = 2.5f;
    static constexpr float MAX_CELL_TEMP_FAULT_V = 60.0f;

    static constexpr uint32_t OVER_VOLTAGE_DEBOUNCE_FAULT_MS = 3000;
    static constexpr uint32_t UNDER_VOLTAGE_DEBOUNCE_FAULT_MS = 3000;
    static constexpr uint32_t OVER_TEMP_DEBOUNCE_FAULT_MS = 3000;
    static constexpr uint32_t COMM_ERR_DEBOUNCE_FAULT_MS = 3000;

    struct CellData {
        float voltage_V;
        float temperature_C;
        uint8_t segment;
        uint8_t cell;
    };

    struct PackStatus {
        float minCellVoltage_V;
        float maxCellVoltage_V;
    };

    ADBMS();

    void init();
    void update();
    void shutdown();

    PackStatus getStatus() const;
    float getCellVoltage(uint8_t segment, uint8_t cell) const;
    float getCellTemperature(uint8_t segment, uint8_t cell) const;

    void balanceCells();
    void checkFaults();

private:
    std::array<std::array<CellData, CELLS_PER_SEGMENT>, NUM_SEGMENTS> cells;
    PackStatus status;

    void readAllVoltages();
    void readAllTemperatures();
    void updatePackStatus();
};

} // namespace app