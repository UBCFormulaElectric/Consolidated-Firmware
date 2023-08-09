#include <stdbool.h>

/**
 * Get the latched fault status of the IMD
 * @return The latched fault status of the IMD
 */
bool Io_LatchedFaults_CheckImdLatchedFault(void);

/**
 * Get the latched fault status of the BSPD
 * @return The latched fault status of the BSPD
 */
bool Io_LatchedFaults_CheckBspdLatchedFault(void);

/**
 * Get the latched fault status of the BMS
 * @return The latched fault status of the BMS
 */
bool Io_LatchedFaults_CheckBmsLatchedFault(void);
