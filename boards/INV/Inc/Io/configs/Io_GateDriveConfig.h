#pragma once

// Config Register 1 (CFG1), bits 7 to 0
#define CRC_SPI 1   // Enable CRC checks
#define UVLOD_EN 0  // Enables under-voltage protection on VDD
#define SD_FLAG 0   // Shutdown pin does not reset status registers
#define DIAG_EN 0   // The IN-/DIAG2 pin works as a PWM input
#define DT_SET 2    // 2 bits, 0 = disabled, 1 = 250ns, 2 = 800ns, 3 = 1200ns
#define IN_FILTER 3 // 2 bits, 0 = disabled, 1 = 160ns, 2 = 500ns, 3 = 70ns

// Config Register 2 (CFG2), bits 7 to 0
#define SENSE_THRES 7 // 3 bits
#define DESAT_CURR 0  // 2 bits, 250uA current source into 150pF capacitor
#define DESAT_THRES 2 // 3 bits, 5V threshold

// Config Register 3 (CFG3), bits 7 to 0
#define TLTO_THRES 0 // 4 bits, 11.00V
#define TLTO_TIME 0  // 4 bits, Two-level turn-off disabled

// Config Register 4 (CFG4), bits 5 to 0, 7-6 unused
#define OVLO_EN 0    // Overvoltage lock-out enabled
#define UVLO_LATCH 1 // Latch undervoltage faults
#define VLON_THRES 1 // 2 bits, Turn-off GDPS rail, -3V undervoltage threshold
#define VHON_THRES 3 // 2 bits, Turn-on GDPS rail, 14V undervoltage threshold

// Config Register 5 (CFG5), bits 3 to 0, 7-4 unused
#define TLTO_EN 1  // Two-level turn-off only active after a fault event
#define SENSE_EN 0 // Current sensing comparator disabled
#define DESAT_EN 1 // Desat comparator enabled
#define CLAMP_EN 0 // Miller clamping disabled

// Diagnostic 1 Register (DIAG1CFG)
#define DIAG1_TWN 0         // Thermal warning
#define DIAG1_TSD 1         // Thermal shutdown
#define DIAG1_ASC 1         // Active short circuit
#define DIAG1_DESAT 1       // Desat event
#define DIAG1_OV 1          // GDPS Overvoltage failure, on/off levels
#define DIAG1_UV 1          // GDPS Undervoltage failure, on/off levels
#define DIAG1_VDD 0         // Vdd PSU failure
#define DIAG1_SPI_REG_ERR 1 // SPI or Register error

// Diagnostic 2 Register (DIAG2CFG) (disabled)
#define DIAG2_TWN 0         // Thermal warning
#define DIAG2_TSD 0         // Thermal shutdown
#define DIAG2_ASC 0         // Active short circuit
#define DIAG2_DESAT 0       // Desat event
#define DIAG2_OV 0          // GDPS Overvoltage failure, on/off levels
#define DIAG2_UV 0          // GDPS Undervoltage failure, on/off levels
#define DIAG2_VDD 0         // Vdd PSU failure
#define DIAG2_SPI_REG_ERR 0 // SPI or Register error

#define STGAP_SPI_TIMEOUT 100 // Timeout for SPI functions
