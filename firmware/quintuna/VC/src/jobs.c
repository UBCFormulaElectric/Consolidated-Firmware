#include "jobs.h"

#include "io_power_monitor.h"

void jobs_init() {}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}

void jobs_pollPwrMtr(void)
{
    // I'm #clueless

    if (io_power_monitor_init())
    {
        app_canTx_VC_Warning_pwrmtrinitFailed_set(1); // Haven't made the CAN message
    }
    float ext_voltage = io_power_monitor_read_voltage(&ext);
    float acc_voltage = io_power_monitor_read_voltage(&acc);
    // TODO: can message to transmit both voltages

    float ext_current = io_power_monitor_read_current(&ext);
    float acc_current = io_power_monitor_read_current(&acc);
    // TODO: can message to transmit both currents

    float ext_power = io_power_monitor_read_power(&ext);
    float acc_power = io_power_monitor_read_power(&acc);
    // TODO: can message to transmit both powers

    uint32_t alert_cause = io_power_monitor_alert_status();
    // TODO: make a warning of sorts for the alert status, maybe a
    // TODO: transmit that tells the user where alert from (based on raised bit)

}
