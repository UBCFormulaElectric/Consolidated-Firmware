#pragma once

#include <stdbool.h>

#define VC_SHDN_NODE_COUNT 4

/*
 * Get the status of the tsms shutdown pin
 * @return the status of the tsms pin set in the shutdown config
 */
bool io_vcShdn_TsmsFault_get(void);

/*
 * Get the status of the LEStop shutdown pin
 * @return the status of the LEStop pin set in the shutdown config
 */
bool io_vcShdn_InertiaSwitch_get(void);

/*
 * Get the status of the REStop shutdown pin
 * @return the status of the REStop pin set in the shutdown config
 */
bool io_vcShdn_RearRightMotorInterlock_get(void);

/*
 * Get the status of the splitter box interlock shutdown pin
 * @return the status of the splitter box interlock shutdown pin set in the shutdown config
 */
bool io_vcShdn_SplitterBoxInterlockFault_get(void);