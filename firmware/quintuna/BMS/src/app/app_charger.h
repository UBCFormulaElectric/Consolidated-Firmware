#pragma once

/**
 * get the avaliable current from the wall charger
 * @return the avaliable current and -1 if invalid PWM duty cycle
 */
float app_charger_getAvaliableCurrent();