#include "hw_gpios.h"
#include "main.h"

const Gpio ams_r_pin  = { .port = AMS_R_GPIO_Port, .pin = AMS_R_Pin };
const Gpio imd_r_pin  = { .port = IMD_R_GPIO_Port, .pin = IMD_R_Pin };
const Gpio bspd_r_pin = { .port = BSPD_R_GPIO_Port, .pin = BSPD_R_Pin };
const Gpio shdn_r_pin = { .port = SHDN_R_GPIO_Port, .pin = SHDN_R_Pin };
const Gpio shdn_g_pin = { .port = SHDN_G_GPIO_Port, .pin = SHDN_G_Pin };

const Gpio vc_r_pin = { .port = VC_R_GPIO_Port, .pin = VC_R_Pin };
const Gpio vc_g_pin = { .port = VC_G_GPIO_Port, .pin = VC_G_Pin };
const Gpio vc_b_pin = { .port = VC_B_GPIO_Port, .pin = VC_B_Pin };

const Gpio bms_r_pin = { .port = BMS_R_GPIO_Port, .pin = BMS_R_Pin };
const Gpio bms_g_pin = { .port = BMS_G_GPIO_Port, .pin = BMS_G_Pin };
const Gpio bms_b_pin = { .port = BMS_B_GPIO_Port, .pin = BMS_B_Pin };

const Gpio fsm_r_pin = { .port = FSM_R_GPIO_Port, .pin = FSM_R_Pin };
const Gpio fsm_g_pin = { .port = FSM_G_GPIO_Port, .pin = FSM_G_Pin };
const Gpio fsm_b_pin = { .port = FSM_B_GPIO_Port, .pin = FSM_B_Pin };

const Gpio rsm_r_pin = { .port = RSM_R_GPIO_Port, .pin = RSM_R_Pin };
const Gpio rsm_g_pin = { .port = RSM_G_GPIO_Port, .pin = RSM_G_Pin };
const Gpio rsm_b_pin = { .port = RSM_B_GPIO_Port, .pin = RSM_B_Pin };

const Gpio aux_r_pin = { .port = AUX_DB_R_GPIO_Port, .pin = AUX_DB_R_Pin };
const Gpio aux_g_pin = { .port = AUX_DB_G_GPIO_Port, .pin = AUX_DB_G_Pin };
const Gpio aux_b_pin = { .port = AUX_DB_B_GPIO_Port, .pin = AUX_DB_B_Pin };

const Gpio crit_r_pin = { .port = CRIT_DB_R_GPIO_Port, .pin = CRIT_DB_R_Pin };
const Gpio crit_g_pin = { .port = CRIT_DB_G_GPIO_Port, .pin = CRIT_DB_G_Pin };
const Gpio crit_b_pin = { .port = CRIT_DB_B_GPIO_Port, .pin = CRIT_DB_B_Pin };

const Gpio start_led_pin            = { .port = START_LED_GPIO_Port, .pin = START_LED_Pin };
const Gpio torque_vectoring_led_pin = { .port = TORQUE_VECTORING_LED_GPIO_Port, .pin = TORQUE_VECTORING_LED_Pin };
const Gpio regen_led_pin            = { .port = REGEN_LED_GPIO_Port, .pin = REGEN_LED_Pin };
const Gpio led_pin                  = { .port = LED_GPIO_Port, .pin = LED_Pin };

const Gpio start_sig_pin            = { .port = START_SIG_GPIO_Port, .pin = START_SIG_Pin };
const Gpio torque_vectoring_sig_pin = { .port = TORQUE_VECTORING_SIG_GPIO_Port, .pin = TORQUE_VECTORING_SIG_Pin };
const Gpio regen_sig_pin            = { .port = REGEN_SIG_GPIO_Port, .pin = REGEN_SIG_Pin };

const Gpio n_drive_mode_0_pin = { .port = NDRIVE_MODE_0b_GPIO_Port, .pin = NDRIVE_MODE_0b_Pin };
const Gpio n_drive_mode_1_pin = { .port = NDRIVE_MODE_1b_GPIO_Port, .pin = NDRIVE_MODE_1b_Pin };
const Gpio n_drive_mode_2_pin = { .port = NDRIVE_MODE_2b_GPIO_Port, .pin = NDRIVE_MODE_2b_Pin };
const Gpio n_drive_mode_3_pin = { .port = NDRIVE_MODE_3b_GPIO_Port, .pin = NDRIVE_MODE_3b_Pin };

const Gpio n_program_pin   = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
const Gpio n_chimera_pin   = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
const Gpio shdn_sen_pin    = { .port = SHDN_SEN_GPIO_Port, .pin = SHDN_SEN_Pin };
const Gpio inertia_sen_pin = { .port = INERTIA_SEN_GPIO_Port, .pin = INERTIA_SEN_Pin };
