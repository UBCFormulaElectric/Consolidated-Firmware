#include "hw_gpios.h"
#include "main.h"
#include "app_utils.h"

namespace hw
{
const gpio start_switch_gpio{ START_SIG_GPIO_Port, START_SIG_Pin };
const gpio regen_switch_gpio{ REGEN_SIG_GPIO_Port, REGEN_SIG_Pin };
const gpio torquevec_switch_gpio{ TORQUE_VECTORING_SIG_GPIO_Port, TORQUE_VECTORING_SIG_Pin };

const gpio imd_r_pin{ IMD_R_GPIO_Port, IMD_R_Pin };
const gpio bspd_r_pin{ BSPD_R_GPIO_Port, BSPD_R_Pin };
const gpio ams_r_pin{ AMS_R_GPIO_Port, AMS_R_Pin };

const gpio start_led_pin{ START_LED_GPIO_Port, START_LED_Pin };
const gpio regen_led_pin{ REGEN_LED_GPIO_Port, REGEN_LED_Pin };
const gpio torquevec_led_pin{ TORQUE_VECTORING_LED_GPIO_Port, TORQUE_VECTORING_LED_Pin };

const gpio shdn_r_pin{ SHDN_R_GPIO_Port, SHDN_R_Pin };
const gpio shdn_g_pin{ SHDN_G_GPIO_Port, SHDN_G_Pin };
const gpio shdn_b_pin_fake{ 0, MAX_8_BITS_VALUE };

const gpio rvc_r_pin{ VC_R_GPIO_Port, VC_R_Pin };
const gpio rvc_g_pin{ VC_G_GPIO_Port, VC_G_Pin };
const gpio rvc_b_pin{ VC_B_GPIO_Port, VC_B_Pin };

const gpio bms_r_pin{ BMS_R_GPIO_Port, BMS_R_Pin };
const gpio bms_g_pin{ BMS_G_GPIO_Port, BMS_G_Pin };
const gpio bms_b_pin{ BMS_B_GPIO_Port, BMS_B_Pin };

const gpio fvc_r_pin{ FSM_R_GPIO_Port, FSM_R_Pin };
const gpio fvc_g_pin{ FSM_G_GPIO_Port, FSM_G_Pin };
const gpio fvc_b_pin{ FSM_B_GPIO_Port, FSM_B_Pin };

const gpio rsm_r_pin{ RSM_R_GPIO_Port, RSM_R_Pin };
const gpio rsm_g_pin{ RSM_G_GPIO_Port, RSM_G_Pin };
const gpio rsm_b_pin{ RSM_B_GPIO_Port, RSM_B_Pin };

const gpio aux_r_pin{ AUX_DB_R_GPIO_Port, AUX_DB_R_Pin };
const gpio aux_g_pin{ AUX_DB_G_GPIO_Port, AUX_DB_G_Pin };
const gpio aux_b_pin{ AUX_DB_B_GPIO_Port, AUX_DB_B_Pin };

const gpio crit_r_pin{ CRIT_DB_R_GPIO_Port, CRIT_DB_R_Pin };
const gpio crit_g_pin{ CRIT_DB_G_GPIO_Port, CRIT_DB_G_Pin };
const gpio crit_b_pin{ CRIT_DB_B_GPIO_Port, CRIT_DB_B_Pin };

const gpio n_drive_mode_0_pin{ NDRIVE_MODE_0b_GPIO_Port, NDRIVE_MODE_0b_Pin };
const gpio n_drive_mode_1_pin{ NDRIVE_MODE_1b_GPIO_Port, NDRIVE_MODE_1b_Pin };
const gpio n_drive_mode_2_pin{ NDRIVE_MODE_2b_GPIO_Port, NDRIVE_MODE_2b_Pin };
const gpio n_drive_mode_3_pin{ NDRIVE_MODE_3b_GPIO_Port, NDRIVE_MODE_3b_Pin };

const gpio cockpit_estop_sen_pin{ SHDN_SEN_GPIO_Port, SHDN_SEN_Pin };
const gpio inertia_sen_pin{ INERTIA_SEN_GPIO_Port, INERTIA_SEN_Pin };
} // namespace hw
