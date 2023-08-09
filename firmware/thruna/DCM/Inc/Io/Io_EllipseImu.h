#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_EllipseImu.h"

/*
 * Initialize the SBG Ellipse N sensor IO module.
 */
bool Io_EllipseImu_Init(void);

/*
 * Parse all logs which are currently residing in the UART RX buffer.
 */
void Io_EllipseImu_HandleLogs(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t Io_EllipseImu_GetTimestampUs(void);

/*
 * Get a last read output from the SBG Ellipse N sensor.
 * This is a single function to avoid faking a whole bunch of IO functions for each sensor output.
 * @param output Which sensor value to read
 * @return Last read sensor output, in the cooresponding units
 */
float Io_EllipseImu_GetSensorOutput(EllipseImuOutput output);

/*
 * Get general status from the sensor.
 * @return Bitmask of faults, 1 indiciates a fault
 */
uint16_t Io_EllipseImu_GetGeneralStatus(void);

/*
 * Get communication status from the sensor.
 * @return Bitmask of faults, 0 indiciates a fault
 */
uint32_t Io_EllipseImu_GetComStatus(void);
