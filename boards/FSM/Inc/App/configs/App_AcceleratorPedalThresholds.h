#pragma once

// TODO: Experimentally determine the encoder values for the primary and
// secondary APPS when the accelerator is fully pressed #666
#define PAPPS_ENCODER_FULLY_PRESSED_VALUE 2500U
#define SAPPS_ENCODER_FULLY_PRESSED_VALUE 1800U

// If encoder exceeds the following values, we reset it back to zero (usually
// due to underflowing)
#define PAPPS_ENCODER_RESET_VALUE 3500U
#define SAPPS_ENCODER_RESET_VALUE 2400U
