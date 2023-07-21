#include "tasks.h"
#include "main.h"
#include "gpio.h"
#include "led.h"
#include "rgbLed.h"
#include "sevenSegDigits.h"
#include "switch.h"
#include "decimalDisplays.h"
#include "globals.h"
#include "avgPower.h"

// Driver data.
static Gpio imd_led_gpio;
static Gpio bspd_led_gpio;
static Gpio shdn_led_gpio;
static Gpio drive_led_gpio;

static Gpio start_switch_gpio;
static Gpio aux_switch_gpio;

static Gpio bms_status_led_red_gpio;
static Gpio bms_status_led_green_gpio;
static Gpio bms_status_led_blue_gpio;

static Gpio dcm_status_led_red_gpio;
static Gpio dcm_status_led_green_gpio;
static Gpio dcm_status_led_blue_gpio;

static Gpio fsm_status_led_red_gpio;
static Gpio fsm_status_led_green_gpio;
static Gpio fsm_status_led_blue_gpio;

static Gpio pdm_status_led_red_gpio;
static Gpio pdm_status_led_green_gpio;
static Gpio pdm_status_led_blue_gpio;

static Gpio dim_status_led_red_gpio;
static Gpio dim_status_led_green_gpio;
static Gpio dim_status_led_blue_gpio;

static Gpio seven_segs_srck_gpio;
static Gpio seven_segs_rck_gpio;
static Gpio seven_segs_ser_out_gpio;
static Gpio seven_segs_dimming_gpio;

// Device data.
static Led imd_led;
static Led bspd_led;
static Led shdn_led;
static Led drive_led;

static Switch start_switch;
static Switch aux_switch;

static RgbLed bms_status_led;
static RgbLed dcm_status_led;
static RgbLed fsm_status_led;
static RgbLed pdm_status_led;
static RgbLed dim_status_led;

void tasks_init()
{
    // Driver initialization.
    gpio_init(&imd_led_gpio, IMD_LED_GPIO_Port, IMD_LED_Pin);
    gpio_init(&bspd_led_gpio, BSPD_LED_GPIO_Port, BSPD_LED_Pin);
    gpio_init(&shdn_led_gpio, SHDN_LED_GPIO_Port, SHDN_LED_Pin);
    gpio_init(&drive_led_gpio, IGNTN_LED_GPIO_Port, IGNTN_LED_Pin);

    gpio_init(&start_switch_gpio, IGNTN_IN_GPIO_Port, IGNTN_IN_Pin);
    gpio_init(&aux_switch_gpio, AUX_IN_GPIO_Port, AUX_IN_Pin);

    gpio_init(&bms_status_led_red_gpio, BMS_RED_GPIO_Port, BMS_RED_Pin);
    gpio_init(&bms_status_led_green_gpio, BMS_GREEN_GPIO_Port, BMS_GREEN_Pin);
    gpio_init(&bms_status_led_blue_gpio, BMS_BLUE_GPIO_Port, BMS_BLUE_Pin);

    gpio_init(&dcm_status_led_red_gpio, DCM_RED_GPIO_Port, DCM_RED_Pin);
    gpio_init(&dcm_status_led_green_gpio, DCM_GREEN_GPIO_Port, DCM_GREEN_Pin);
    gpio_init(&dcm_status_led_blue_gpio, DCM_BLUE_GPIO_Port, DCM_BLUE_Pin);

    gpio_init(&fsm_status_led_red_gpio, FSM_RED_GPIO_Port, FSM_RED_Pin);
    gpio_init(&fsm_status_led_green_gpio, FSM_GREEN_GPIO_Port, FSM_GREEN_Pin);
    gpio_init(&fsm_status_led_blue_gpio, FSM_BLUE_GPIO_Port, FSM_BLUE_Pin);

    gpio_init(&pdm_status_led_red_gpio, PDM_RED_GPIO_Port, PDM_RED_Pin);
    gpio_init(&pdm_status_led_green_gpio, PDM_GREEN_GPIO_Port, PDM_GREEN_Pin);
    gpio_init(&pdm_status_led_blue_gpio, PDM_BLUE_GPIO_Port, PDM_BLUE_Pin);

    gpio_init(&dim_status_led_red_gpio, DIM_RED_GPIO_Port, DIM_RED_Pin);
    gpio_init(&dim_status_led_green_gpio, DIM_GREEN_GPIO_Port, DIM_GREEN_Pin);
    gpio_init(&dim_status_led_blue_gpio, DIM_BLUE_GPIO_Port, DIM_BLUE_Pin);

    gpio_init(seven_segs_srck_gpio, SEVENSEGS_SRCK_GPIO_Port, SEVENSEGS_SRCK_Pin);
    gpio_init(seven_segs_rck_gpio, SEVENSEGS_RCK_GPIO_Port, SEVENSEGS_RCK_Pin);
    gpio_init(seven_segs_ser_out_gpio, SEVENSEGS_SEROUT_GPIO_Port, SEVENSEGS_SEROUT_Pin);
    gpio_init(seven_segs_dimming_gpio, SEVENSEGS_DIMMING_GPIO_Port, SEVENSEGS_DIMMING_Pin);

    // Device initialization.
    led_init(&imd_led, &imd_led_gpio);
    led_init(&bspd_led, &bspd_led_gpio);
    led_init(&shdn_led, &shdn_led_gpio);
    led_init(&drive_led, &drive_led_gpio);

    switch_init(&start_switch, &start_switch_gpio, true);
    switch_init(&aux_switch, &aux_switch_gpio, true);

    rgbLed_init(&bms_status_led, &bms_status_led_red_gpio, &bms_status_led_green_gpio, &bms_status_led_blue_gpio);
    rgbLed_init(&dcm_status_led, &dcm_status_led_red_gpio, &dcm_status_led_green_gpio, &dcm_status_led_blue_gpio);
    rgbLed_init(&fsm_status_led, &fsm_status_led_red_gpio, &fsm_status_led_green_gpio, &fsm_status_led_blue_gpio);
    rgbLed_init(&pdm_status_led, &pdm_status_led_red_gpio, &pdm_status_led_green_gpio, &pdm_status_led_blue_gpio);
    rgbLed_init(&dim_status_led, &bms_status_led_red_gpio, &bms_status_led_green_gpio, &bms_status_led_blue_gpio);

    sevenSegDigits_init(&seven_segs_srck_gpio, &seven_segs_rck_gpio, &seven_segs_ser_out_gpio, &seven_segs_dimming_gpio);

    // App initialization.
    avgPower_init();
    decimalDisplays_init();
    globals_init(&imd_led, &bspd_led, &shdn_led, &drive_led, &start_switch, &aux_switch, &bms_status_led, &dcm_status_led, &fsm_status_led, &pdm_status_led, dim_status_led);
}

void tasks_runTaskStateMachine(void)
{
    // TODO
}

void tasks_runTaskCanTx(void)
{
    // TODO
}