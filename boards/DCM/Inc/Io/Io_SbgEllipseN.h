#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_SbgEllipseN.h"

/*
 * Initialize the SBG Ellipse N sensor IO module.
 */
bool Io_SbgEllipseN_Init(void);

/*
 * Parse all logs which are currently residing in the UART RX buffer.
 */
void Io_SbgEllipseN_HandleLogs(void);

/*
 * Get the last received timestamp.
 * @return Timestamp in us
 */
uint32_t Io_SbgEllipseN_GetTimestampUs(void);

/*
 * Get a last read output from the SBG Ellipse N sensor.
 * This is a single function to avoid faking a whole bunch of IO functions for each sensor output.
 * @param output Which sensor value to read
 * @return Last read sensor output, in the cooresponding units
 */
float Io_SbgEllipseN_GetSensorOutput(SbgSensorOutput output);

/*
 * Whether or not sensor's status is OK.
 * @return True if OK, false if not
 */
bool Io_SbgEllipseN_StatusOk(void);
