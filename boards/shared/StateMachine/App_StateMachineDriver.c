/******************************************************************************
 * Includes
 ******************************************************************************/
#include <sched.h>
#include "App_StateMachine.h"
#include "App_Macros.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void App_StateMachine_TransitionState(
    Event                             event,
    State *                           current_state,
    uint32_t                          num_of_transitions,
    StateTransitionTableEntry_Struct *state_table)
{
    // Get the table entry corresponding to the current_state
    StateTransitionTableEntry_Struct current_state_entry =
        state_table[*current_state];

    // Iteratively search through all possible state transitions. The size of
    // this array should be small so the search should not take too long.
    for (uint32_t i = 0; i < num_of_transitions; i++)
    {
        // Check if the event requires us to transition out of the current state
        if (event == current_state_entry.transitions[i].trigger_event)
        {
            // Call on-exit function for current state
            if (current_state_entry.run_on_exit != NULL)
            {
                current_state_entry.run_on_exit();
            }

            // Transition to new state
            *current_state = current_state_entry.transitions[i].next_state;

            // Find the table entry to new state
            StateTransitionTableEntry_Struct new_state_entry =
                state_table[current_state_entry.transitions[i].next_state];

            // Call on-enter function for new state
            if (new_state_entry.run_on_entry != NULL)
            {
                new_state_entry.run_on_entry();
            }

            // Exit the for-loop early if we've completed a state transition
            break;
        }
    }
}
