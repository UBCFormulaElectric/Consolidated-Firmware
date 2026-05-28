#pragma once

namespace app::sd
{
    /**
    * Initialise the filesystem object and log the file descriptor for the file.
    */
    void init_fs();

    /**
     * Updates the metadata (time) of the given file descriptor with the current time from the RTC.
     * Writes year, month, day, hours, minutes, seconds in the first 6 bytes of the metadata block.
     */
    void update_metadata();

    /**
     * Returns the open log file descriptor.
     */
    uint32_t getLogFd();

    /**
     * Returns true if the log file is currently open.
     */
    bool isLogOpen();
}