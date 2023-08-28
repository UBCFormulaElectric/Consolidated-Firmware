#include "main.h"
#include "dev_sevenSegDisplays.h"
#include "dev_led.h"
#include "dev_rgbLed.h"
#include "dev_binarySwitch.h"
#include "hw_gpio.h"

// BMS RGB LED pins.
static const Gpio bms_red_pin = {
    .port = BMS_RED_GPIO_Port,
    .pin = BMS_RED_Pin,
};
static const Gpio bms_green_pin = {
    .port = BMS_GREEN_GPIO_Port,
    .pin = BMS_GREEN_Pin,
};
static const Gpio bms_blue_pin = {
    .port = BMS_BLUE_GPIO_Port,
    .pin = BMS_BLUE_Pin,
};

// DCM RGB LED pins.
static const Gpio dcm_red_pin = {
    .port = DCM_RED_GPIO_Port,
    .pin = DCM_RED_Pin,
};
static const Gpio dcm_green_pin = {
    .port = DCM_GREEN_GPIO_Port,
    .pin = DCM_GREEN_Pin,
};
static const Gpio dcm_blue_pin = {
    .port = DCM_BLUE_GPIO_Port,
    .pin = DCM_BLUE_Pin,
};

// FSM RGB LED pins.
static const Gpio fsm_red_pin = {
    .port = FSM_RED_GPIO_Port,
    .pin = FSM_RED_Pin,
};
static const Gpio fsm_green_pin = {
    .port = FSM_GREEN_GPIO_Port,
    .pin = FSM_GREEN_Pin,
};
static const Gpio fsm_blue_pin = {
    .port = FSM_BLUE_GPIO_Port,
    .pin = FSM_BLUE_Pin,
};

// PDM RGB LED pins.
static const Gpio pdm_red_pin = {
    .port = PDM_RED_GPIO_Port,
    .pin = PDM_RED_Pin,
};
static const Gpio pdm_green_pin = {
    .port = PDM_GREEN_GPIO_Port,
    .pin = PDM_GREEN_Pin,
};
static const Gpio pdm_blue_pin = {
    .port = PDM_BLUE_GPIO_Port,
    .pin = PDM_BLUE_Pin,
};

// DIM RGB LED pins.
static const Gpio dim_red_pin = {
    .port = DIM_RED_GPIO_Port,
    .pin = DIM_RED_Pin,
};
static const Gpio dim_green_pin = {
    .port = DIM_GREEN_GPIO_Port,
    .pin = DIM_GREEN_Pin,
};
static const Gpio dim_blue_pin = {
    .port = DIM_BLUE_GPIO_Port,
    .pin = DIM_BLUE_Pin,
};

// Binary LED pins.
static const Gpio imd_led_pin = {
    .port = IMD_LED_GPIO_Port,
    .pin = IMD_LED_Pin,
};
static const Gpio bspd_led_pin = {
    .port = BSPD_LED_GPIO_Port,
    .pin = BSPD_LED_Pin,
};
static const Gpio shdn_led_pin = {
    .port = SHDN_LED_GPIO_Port,
    .pin = SHDN_LED_Pin,
};
static const Gpio drive_led_pin = {
    .port = IGNTN_LED_GPIO_Port,
    .pin = IGNTN_LED_Pin,
};
static const Gpio aux_led_pin = {
    .port = AUX_LED_GPIO_Port,
    .pin = AUX_LED_Pin,
};

// Binary switch pins.
static const Gpio start_switch_pin = {
    .port = IGNTN_IN_GPIO_Port,
    .pin = IGNTN_IN_Pin,
};
static const Gpio aux_switch_pin = {
    .port = AUX_IN_GPIO_Port,
    .pin = AUX_IN_Pin,
};

// 7-segment displays pins.
static const Gpio seven_segs_srck_pin = {
    .port = SEVENSEGS_SRCK_GPIO_Port,
    .pin = SEVENSEGS_SRCK_Pin,
};
static const Gpio seven_segs_rck_pin = {
    .port = SEVENSEGS_RCK_GPIO_Port,
    .pin = SEVENSEGS_RCK_Pin,
};
static const Gpio seven_segs_serout_pin = {
    .port = SEVENSEGS_SEROUT_GPIO_Port,
    .pin = SEVENSEGS_SEROUT_Pin,
};
static const Gpio seven_segs_dimming_pin = {
    .port = SEVENSEGS_DIMMING_GPIO_Port,
    .pin = SEVENSEGS_DIMMING_Pin,
};

// RGB LEDs.
static const RgbLed bms_rgb_led = {
    .red_enable = &bms_red_pin,
    .green_enable = &bms_green_pin,
    .blue_enable = &bms_blue_pin,
};
static const RgbLed dcm_rgb_led = {
    .red_enable = &dcm_red_pin,
    .green_enable = &dcm_green_pin,
    .blue_enable = &dcm_blue_pin,
};
static const RgbLed fsm_rgb_led = {
    .red_enable = &fsm_red_pin,
    .green_enable = &fsm_green_pin,
    .blue_enable = &fsm_blue_pin,
};
static const RgbLed pdm_rgb_led = {
    .red_enable = &pdm_red_pin,
    .green_enable = &pdm_green_pin,
    .blue_enable = &pdm_blue_pin,
};
static const RgbLed dim_rgb_led = {
    .red_enable = &dim_red_pin,
    .green_enable = &dim_green_pin,
    .blue_enable = &dim_blue_pin,
};

// Binary LEDs.
static const Led imd_led = {
    .pin = &imd_led_pin
};
static const Led bspd_led = {
    .pin = &bspd_led_pin
};
static const Led shdn_led = {
    .pin = &shdn_led_pin
};
static const Led drive_led = {
    .pin = &drive_led_pin
};

// Binary switches.
static const BinarySwitch start_switch = {
    .pin = &start_switch_pin,
    .on_state = true,
};

static const BinarySwitch aux_switch = {
    .pin = &aux_switch_pin,
    .on_state = true,
};

// 7-segment displays config.
static const SevenSegDigitsConfig seven_segs_config = {
    .srck_pin = &seven_segs_srck_pin,
    .rck_pin = &seven_segs_rck_pin,
    .ser_out_pin = &seven_segs_serout_pin,
    .dimming_pin = &seven_segs_dimming_pin,
};

void hw_tasks_init() 
{
    dev_sevenSegDigits_init(&seven_segs_config);
}
