#pragma once

// Keep CAN protocol in sync with:
// canup/bootloader.py

// Command IDs.
#define ERASE_SECTOR_ID 1000
#define PROGRAM_ID 1001
#define VERIFY_ID 1002

// Reply IDs.
#define ERASE_SECTOR_COMPLETE_ID 1010
#define APP_VALIDITY_ID 1011

#if defined(BMS)

#define START_UPDATE_ID 1100
#define UPDATE_ACK_ID 1101
#define STATUS_10HZ_ID 1109
#define BOOT_GPIO_PORT GPIOA
#define BOOT_GPIO_PIN GPIO_PIN_2

#elif defined(DCM)

#define START_UPDATE_ID 1110
#define UPDATE_ACK_ID 1111
#define STATUS_10HZ_ID 1119
#define BOOT_GPIO_PORT GPIOB
#define BOOT_GPIO_PIN GPIO_PIN_12

#elif defined(FSM)

#define START_UPDATE_ID 1120
#define UPDATE_ACK_ID 1121
#define STATUS_10HZ_ID 1129
#define BOOT_GPIO_PORT GPIOB
#define BOOT_GPIO_PIN GPIO_PIN_12

#elif defined(PDM)

#define START_UPDATE_ID 1130
#define UPDATE_ACK_ID 1131
#define STATUS_10HZ_ID 1139
#define BOOT_GPIO_PORT GPIOA
#define BOOT_GPIO_PIN GPIO_PIN_9

#elif defined(DIM)

#define START_UPDATE_ID 1140
#define UPDATE_ACK_ID 1141
#define STATUS_10HZ_ID 1149
#define BOOT_GPIO_PORT GPIOB
#define BOOT_GPIO_PIN GPIO_PIN_4

#else

#error "Board not defined!"

#endif