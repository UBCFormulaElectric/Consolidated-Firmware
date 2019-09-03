/**
 * @brief Application level code for state machine logic
 */
#ifndef APP_STATE_MACHINE_H
#define APP_STATE_MACHINE_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedStateMachine.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Getter function for state table.
 * @return Pointer to state table.
 */
StateTransitionTableEntry_Struct *App_StateMachine_GetStateTable(void);

/**
 * @brief  Getter function for current state.
 * @return Current state.
 */
State App_StateMachine_GetCurrentState(void);

/**
 * @brief  Perform different actions depending on the current state. This
 *         function should be called in a periodic loop.
 */
void App_StateMachine_RunPeriodic(void);

#endif /* APP_STATE_MACHINE_H */
