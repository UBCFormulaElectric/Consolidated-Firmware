/*
 * Error Handling Strategy:
 * =========================
 *  (1) An error is detected in some source file (e.g. In CheckHeartbeat(), we
 * expected a heartbeat but does not receive it)
 *  (2) CheckHeartbeat() is responsible to increment the 'current' variable in
 * consecutive_errors[MISSING_HEARTBEAT] using
 * IncrementNumberOfConsecutiveErrors()
 *  (3) After X amount of time, ErrorHandlingRoutine() is called in a time-base
 * triggered ISR
 *  (4) ErrorHandlingRoutine() checks if (i.e.
   consecutive_errors[MISSING_HEARTBEAT].current >=
   consecutive_errors[MISSING_HEARTBEAT].threshold).
        (#) If true,
            (##) populate the one-hot-encoded array and transmit it over a CAN
                 message
            (##) Clear consecutive_errors[MISSING_HEARTBEAT].current
*/
/******************************************************************************
* Includes
*******************************************************************************/
#include "SharedErrorHandling.h"
#include "Can.h"
#include "ErrorHandling.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define NUM_ERRORS 1

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct
{
  uint32_t current;
  uint32_t threshold;
} Error_Struct;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
Error_Struct consecutive_errors[NUM_ERRORS] = {0};

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
// TODO: Correct the array size

void ErrorHandling_InitializeConsecutiveErrors(void) {
    consecutive_errors[SYSTICK_INITIALISATION_ERROR].threshold = 3;
    consecutive_errors[MISSING_HEARTBEAT].threshold            = 3;
}

void IncrementNumberOfConsecutiveErrors(Error_Enum error) {
    consecutive_errors[error].current++;
}

// To be triggered by a time-base interrupt or by a thread
void ErrorHandlingRoutine(void) {
    // Use uint64_t to match the CAN payload size
    uint64_t one_hot_active_errors = 0;

    // Populate one-hot encoded errors and handle errors as needed
    for (uint32_t error = 0; error < NUM_ERRORS; error++) {
        // If the consecutive error threshold has been met
        if (consecutive_errors[error].current >=
            consecutive_errors[error].threshold) {
            // Record error in one-hot encoded errors
            one_hot_active_errors |= 1 << error;

            // Clear consecutive errors count
            consecutive_errors[error].current = 0;

            // Handle error
            HandleError(error);
        }
    }

    // Convert one-hot encoded errors to unit8_t array
    uint8_t can_payload[sizeof(one_hot_active_errors)];
    can_payload[0] = (uint8_t)(one_hot_active_errors >> 0);
    can_payload[1] = (uint8_t)(one_hot_active_errors >> 8);
    can_payload[2] = (uint8_t)(one_hot_active_errors >> 16);
    can_payload[3] = (uint8_t)(one_hot_active_errors >> 24);
    can_payload[4] = (uint8_t)(one_hot_active_errors >> 32);
    can_payload[5] = (uint8_t)(one_hot_active_errors >> 40);
    can_payload[6] = (uint8_t)(one_hot_active_errors >> 48);
    can_payload[7] = (uint8_t)(one_hot_active_errors >> 56);

    // If we have any active error in the one-hot encoded array
    uint8_t dummy_zeroes[sizeof(one_hot_active_errors)] = {0};
    if (memcmp(can_payload, dummy_zeroes, sizeof(one_hot_active_errors))) {
        // Transmit the one-hot encoded errors over CAN
        SharedCAN_TransmitDataCAN(&can_headers[PDM_ERROR],
                                  can_payload);
    }
}

__weak void HandleError(Error_Enum Error) {}
