#pragma once

struct Led *
     App_Led_Create(void (*turn_on_led)(void), void (*turn_off_led)(void));
void App_Led_Destroy(struct Led *led);
void App_Led_TurnOn(const struct Led *led);
void App_Led_TurnOff(const struct Led *led);
