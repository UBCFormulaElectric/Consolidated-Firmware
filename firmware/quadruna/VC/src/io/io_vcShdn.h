#pragma once
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio tsms_gpio;
    const Gpio LE_stop_gpio;
    const Gpio RE_stop_gpio;
    const Gpio splitter_box_interlock_gpio;
} VcShdnConfig;
#else
EMPTY_STRUCT(VcShdnConfig);
#endif

#define VcShdnNodeCount 4

/*
 * Initialize config for shutdown gpio pins
 * @param config with shutdown gpio pins
 */
void io_vcShdn_init(const VcShdnConfig *shutdown_config);

/*
 * Get the status of the tsms shutdown pin
 * @return the status of the tsms pin set in the shutdown config
 */
bool io_vcShdn_TsmsFault_get(void);

/*
 * Get the status of the LEStop shutdown pin
 * @return the status of the LEStop pin set in the shutdown config
 */
bool io_vcShdn_LEStopFault_get(void);

/*
 * Get the status of the REStop shutdown pin
 * @return the status of the REStop pin set in the shutdown config
 */
bool io_vcShdn_REStopFault_get(void);

/*
 * Get the status of the splitter box interlock shutdown pin
 * @return the status of the splitter box interlock shutdown pin set in the shutdown config
 */
bool io_vcShdn_SplitterBoxInterlockFault_get(void);
