/**
 * @file io_ltc6813_internal.h
 * @note this file SHOULD NOT BE imported outside of ltc drivers.
 */
#pragma once
#include "utils.h"
#include <stdint.h>
#include <stdbool.h>

#define REGISTER_GROUP_SIZE (6U) // 6 bytes
#define PEC_SIZE (2U)            // 2 bytes

// we wrote it this way to make hide the little endian processor storage of the struct
// this way, the mental model is that the data is stored in the order it is written (big endian)
// I know about scalar_storage_order("big-endian") but you cannot get the address a struct with that attribute
// that is problematic for pec15 finding
// see table 30
typedef struct __attribute__((__packed__))
{
    uint8_t pec_0;
    uint8_t pec_1;
} PEC;

/**
 * Given a buffer of data, builds a pec object for it, constructor of raw_pec
 * @param data data to encode
 * @param len length of data to encode
 * @return the pec buffer
 */
PEC io_ltc6813_build_data_pec(const uint8_t *data, uint8_t len);

/**
 * checks data against the given pec to see if they match
 * @param data data to check
 * @param len length of the data to check
 * @param pec pec to check against
 * @return if they match
 */
bool io_ltc6813_check_pec(const uint8_t *data, uint8_t len, const PEC *pec);

// see above about the struct nonsense
// see table 35
typedef struct __attribute__((__packed__))
{
    uint8_t cmd_0;
    uint8_t cmd_1;
    PEC     pec;
} ltc6813_tx;

/**
 * @param command Constructor for raw_cmd
 * @return Raw command, with the appropriate command and pec
 */
ltc6813_tx io_ltc6813_build_tx_cmd(uint16_t command);

/**
 * sends the given command
 * @note I am debating on removing this? cause this file seems to be a no side effects type beat,
 * could even just be a header file (inline everything)
 * @param command command to send
 * @return success of operation
 */
ExitCode io_ltc6813_sendCommand(uint16_t command);
