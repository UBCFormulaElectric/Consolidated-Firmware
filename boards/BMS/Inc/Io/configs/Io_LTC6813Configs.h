#pragma once

#define NUM_OF_CMD_BYTES 4U
#define NUM_OF_RX_BYTES 8U
#define NUM_OF_PEC15_BYTES_PER_CMD 2U

#define SPI_INTERFACE_TIMEOUT_MS_LTC6813 2U

// ADC timeout threshold of 10 was chosen arbitrarily.
// TODO: Determine the ADC conversion timeout threshold #674
#define ADC_TIMEOUT_CYCLES_THRESHOLD 10U

#define MD 1U
#define DCP 0U
#define CH 0U
#define CHG 0U
#define CHST 0U
