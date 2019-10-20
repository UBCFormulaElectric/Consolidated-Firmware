//
// Created by pasit on 2019-10-19.
//

#ifndef DCM_APP_SOC_H
#define DCM_APP_SOC_H
#include <stdint.h>
/**
 * @brief       Majority logic decision
 * @return      Ptr to 4 byte packets, else return null if error
 */
uint8_t *majorityLogicDecision(void);

/**
 * @brief      Write data to a byte address and reads back the data
 * @param      None
 * @return     None
 */
void App_SoC_writeSoc(void);

/**
 */
void App_SoC_ReadSoC(void);

#endif //DCM_APP_SOC_H
