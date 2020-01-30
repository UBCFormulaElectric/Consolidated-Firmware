/**
 * @brief Shared library for the state machine framework
 */

#ifndef SHARED_STATEMACHINE_H
#define SHARED_STATEMACHINE_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @brief Standardized data type for states */
typedef uint32_t State;

/** @brief Standardized data type for events */
typedef uint32_t Event;

/** @brief Function pointer for the state action */
typedef void (*StateAction_Fn)(void);

/** @brief Describes an event-driven state transition */
struct StateTransition
{
    Event trigger_event;
    State next_state;
};

/** @brief Function pointer for on-enter and on-exit functions */
typedef void (*Transition_Fn)(void);

/** @brief An entry in the state table */
struct StateTransitionTableEntry
{
    const struct StateTransition *const transitions;
    const Transition_Fn                 run_on_entry;
    const Transition_Fn                 run_on_exit;
    const StateAction_Fn                run_state_action;
};

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Update the state machine given an event.
 * @param event An event that can potentially trigger a state transition.
 * @param current_state Current state of the state machine.
 * @param num_of_transitions Number of possible state transitions for the
 *        current state
 * @param table A state table.
 */
void SharedStateMachine_TransitionState(
    Event                             event,
    State *                           current_state,
    uint32_t                          num_of_transitions,
    struct StateTransitionTableEntry *table);

#endif /* SHARED_STATEMACHINE_H */
