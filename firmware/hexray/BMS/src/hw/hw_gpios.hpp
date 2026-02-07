#pragma once

#include "hw_gpio.hpp"

namespace hw::gpio
{
extern const Gpio led_b;
extern const Gpio shdn_diag_en;
extern const Gpio bspd_ok_current_status;
extern const Gpio accel_brake_ok;
extern const Gpio n_brake_press_3v3;
extern const Gpio n_high_current_bspd;
extern const Gpio msd_shdn_sns;
extern const Gpio hv_p_intlck_sns;
extern const Gpio hv_n_intlck_sns;
extern const Gpio imd_latch;
extern const Gpio diag_ts;
extern const Gpio sd_cd;
extern const Gpio led_r;
extern const Gpio spi_cs_ls;
extern const Gpio tsense_sel0;
extern const Gpio tsense_sel1;
extern const Gpio tsense_sel2;
extern const Gpio bspd_test_enable;
extern const Gpio ts_isense_ocsc_ok;
extern const Gpio spi_cs_hs;
extern const Gpio bms_ok_current_status;
extern const Gpio n_evse_i_lim;
extern const Gpio ir_p_en;
extern const Gpio shdn_en;
extern const Gpio shdn_fault;
extern const Gpio n_bms_latch_status;
extern const Gpio imd_ok_current_status;
extern const Gpio n_bspd_latch_status;
extern const Gpio precharge_en;
extern const Gpio shdn_ol_on;
extern const Gpio fan_en;
extern const Gpio fan_pgood;
extern const Gpio pgood_3v3;
extern const Gpio pgood_5v;
} // namespace hw::gpio