#include "io_pumpControl.h"

#include "hw_i2cs.h"
#include "io_efuses.h"
#include "io_pump.h"

#ifdef TARGET_EMBEDDED
static const Potentiometer pumps_pot = { .i2c_handle = &pumps };
#else
static const Potentiometer pumps_pot = { 0 };
#endif

static PumpConfig rr_pump_config;
static PumpConfig rl_pump_config;
static PumpConfig f_pump_config;

static PumpConfig *pump_table[PUMP_COUNT] = {
    [RR_PUMP] = &rr_pump_config,
    [RL_PUMP] = &rl_pump_config,
    [F_PUMP]  = &f_pump_config,
};

void io_pumpControl_init(void)
{
    rr_pump_config = (PumpConfig){
        .pot    = &pumps_pot,
        .wiper  = WIPER1,
        .invert = false,
        .efuse  = &rr_pump_efuse
    };

    f_pump_config = (PumpConfig){
        .pot    = &pumps_pot,
        .wiper  = WIPER0,
        .invert = false,
        .efuse  = &f_pump_efuse
    };

    rl_pump_config = (PumpConfig){ 
        .pot    = &pumps_pot,
        .wiper  = WIPER0,
        .invert = true, //rsm therefore invert 
        .efuse  = &rl_pump_efuse
    };

    io_pumps_registerConfig(pump_table, PUMP_COUNT);
}

ExitCode io_pumpControl_setPercentage(uint8_t percentage, PumpID pump)
{
    return io_pump_setPercent(pump, percentage);
}

ExitCode io_pumpControl_readPercentage(uint8_t *percentage, PumpID pump)
{
    return io_pump_getPercent(pump, percentage);
}
