/******************************************************************************
 * Includes
 ******************************************************************************/
#include "App_StateMachine.h"
#include <stddef.h>

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off
/** @brief Events that may trigger a state transition */
#define EVENT_READY   (Event)(0)
#define EVENT_FAULT   (Event)(1)
#define EVENT_RECOVER (Event)(2)

/** @brief Possible states of the state machine */
#define STATE_INIT  (State)(0)
#define STATE_DRIVE (State)(1)
#define STATE_FAULT (State)(2)
#define NUM_STATES  (uint32_t)(3)
// clang-format on

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Execute right before we exit the INIT state
 */
static void InitStateExitAction(void);

/**
 * @brief  INIT state action
 */
static void InitStateAction(void);

/**
 * @brief  Execute right before we enter the DRIVE state
 */
static void DriveStateEntryAction(void);

/**
 * @brief  Execute right before we exit the DRIVE state
 */
static void DriveStateExitAction(void);

/**
 * @brief  DRIVE state action
 */
static void DriveStateAction(void);

/**
 * @brief  Execute right before we enter the FAULT state
 */
static void FaultStateEntryAction(void);

/**
 * @brief  Execute right before we exit the FAULT state
 */
static void FaultStateExitAction(void);

/**
 * @brief  FAULT state action
 */
static void FaultStateAction(void);

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
/** @brief Current state of the state machine */
static State current_state = STATE_INIT;

/** @brief Table of information required for the state machine to work*/
static struct StateTransitionTableEntry state_table[NUM_STATES] =
{
    [STATE_INIT] =
    {
        .transitions = (struct StateTransition[])
        {
            {0,0},
        },
        // There is no support for executing on-enter function for the INIT
        // state. And we don't plan to support it because there's no use-case.
        .run_on_entry = NULL,
        .run_on_exit = InitStateExitAction,
        .run_state_action = InitStateAction,
    },
    [STATE_DRIVE] =
    {
        .transitions = (struct StateTransition[])
        {
            {0,0},
        },
        .run_on_entry = DriveStateEntryAction,
        .run_on_exit = DriveStateExitAction,
        .run_state_action = DriveStateAction,
    },
    [STATE_FAULT] =
    {
        .transitions = (struct StateTransition[])
        {
            {0,0},
        },
        .run_on_entry = FaultStateEntryAction,
        .run_on_exit = FaultStateExitAction,
        .run_state_action = FaultStateAction,
    },
};

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void InitStateExitAction(void) {}

static void InitStateAction(void) {}

static void DriveStateEntryAction(void) {}

static void DriveStateExitAction(void) {}

static void DriveStateAction(void) {}

static void FaultStateEntryAction(void) {}

static void FaultStateExitAction(void) {}

static void FaultStateAction(void) {}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
struct StateTransitionTableEntry *App_StateMachine_GetStateTable(void)
{
    return &state_table[0];
}

State App_StateMachine_GetCurrentState(void)
{
    return current_state;
}

void App_StateMachine_RunPeriodic(void)
{
    state_table[current_state].run_state_action();
}
