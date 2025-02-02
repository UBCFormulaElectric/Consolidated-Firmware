#include "hw_pwms.h"
#include "main.h"

const PwmInputFreqOnlyConfig flow_meter_config = { .htim                = &htim4,
                                                   .tim_auto_reload_reg = 0,
                                                   .tim_channel         = 0,
                                                   .tim_frequency_hz    = 0 };

const PwmInputFreqOnly flow_meter_5v5 = { .config             = &flow_meter_config,
                                          .curr_rising_edge   = 0,
                                          .first_tick         = 0,
                                          .frequency_hz       = 0,
                                          .prev_rising_edge   = 0,
                                          .tim_overflow_count = 0 };