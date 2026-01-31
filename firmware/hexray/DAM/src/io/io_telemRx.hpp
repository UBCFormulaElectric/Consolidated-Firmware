#pragma once

namespace io::telemRx
{

/**
 * Process incoming telemetry data from UART.
 * Parses time sync messages and updates RTC.
 * Called in a loop from the TelemRx task.
 */
void run();

} // namespace io::telemRx
