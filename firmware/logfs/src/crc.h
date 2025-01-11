#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

    /**
     * Stamp the first 32 bits of a block with a CRC32 checksum over the data. Used for error detection (all blocks on
     * disk reserve the first 32 bits for a checksum).
     *
     * @param fs Filesystem object.
     * @param buf 1-block sized buffer to stamp with a checksum.
     */
    void crc_stampBlock(const LogFs *fs, void *buf);

    /**
     * Check if the first 32 bits of a block contain a valid CRC32 checksum that matches the data in the buffer. Used
     * for error detection (all blocks on disk reserve the first 32 bits for a checksum).
     *
     * @param fs Filesystem object.
     * @param buf 1-block sized buffer to check the checksum of.
     * @return true if the checksum matches, false otherwise.
     */
    bool crc_checkBlock(const LogFs *fs, void *buf);

#ifdef __cplusplus
}
#endif
