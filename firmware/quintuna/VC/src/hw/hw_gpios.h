#include "hw_gpio.h"

namespace hw::gpio
{
// Power control GPIOs
extern const Gpio buzzer_pwr_en;
extern const Gpio telem_pwr_en;
extern const Gpio npcm_en;
extern const Gpio lv_pwr_en;
extern const Gpio aux_pwr_en;
extern const Gpio pump_pwr_en;
extern const Gpio inv_l_pwr_en;
extern const Gpio inv_r_pwr_en;
extern const Gpio shdn_pwr_en;
extern const Gpio fr_stby1;
extern const Gpio fr_stby2;
extern const Gpio fr_stby3;

// Sensor GPIOs
extern const Gpio bat_i_sns_nflt;
extern const Gpio acc_i_sns_nflt;
extern const Gpio pgood;
extern const Gpio l_shdn_sns;
extern const Gpio r_shdn_sns;
extern const Gpio tsms_shdn_sns;

// Special GPIOs
extern Gpio       sd_present;
extern const Gpio _900k_gpio;
extern const Gpio nchrg_fault;
extern const Gpio nchrg;
extern const Gpio n_chimera_pin;
extern const Gpio nprogram_3v3;
extern const Gpio sb_ilck_shdn_sns;
} // namespace hw::gpio
