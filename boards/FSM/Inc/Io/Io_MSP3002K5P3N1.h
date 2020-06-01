#pragma once

enum BrakeStatus
{
    BRAKE_INACTIVE,
    BRAKE_ACTIVE
};

/**
 * Get the brake actuation status from the MSP-300-2K5-P-3-N-1 brake pressure
 * sensor
 * @return The actuation status of the brake
 */
enum BrakeStatus Io_MSP3002K5P3N1_GetBrakeStatus(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the brake pressure sensor in psi
 */
float Io_MSP3002K5P3N1_GetPressurePsi(void);
