/**
 * @file  CanTest.h
 * @brief Our infrastructure for running tests over CAN
 */
#ifndef CAN_TEST_H
#define CAN_TEST_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "SharedCan.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

// Create a test with given ID
// TODO: We should be explicitly checking the test_id is valid `uint16_t`
#define CANTEST_TEST(TEST_ID) \ 
    void CanTest_Test_##TEST_ID(CanTest_TestResult* test_status)

// Assert a given condition within a CANTEST_TEST. If the condition is false,
// then set the appropriate flags and exit the test
#define CANTEST_ASSERT(COND) \
    if (!( COND )) { \
        test_status->test_passed = false; \
        return; \
    }

// Set the extra values that any test can return.
// TODO: explicitly cast these values to uint8_t?
#define CANTEST_SET_OPTIONAL_VALUES(expr0, expr1, expr2, expr3) \
    test_status->extra_values[0] = expr0; \
    test_status->extra_values[1] = expr1; \
    test_status->extra_values[2] = expr2; \
    test_status->extra_values[3] = expr3;

// Start a list of tests (usually within CanTest_runTest)
#define CANTEST_START_TESTS \
    CanTest_TestResult test_status = \
    { \
        .valid_test = true, \
        .test_passed = true, \
        .extra_values = { 0, 0, 0, 0 } \
    }; \
    switch(test_id) {

// Add a test with the given ID to a list of tests
#define CANTEST_ADD_TEST(TEST_ID) \
        case TEST_ID: \
            CanTest_Test_##TEST_ID(&test_status); \
            break;

// End a list of tests (usually within CanTest_runTest)
#define CANTEST_END_TESTS \
        default: \
            test_status.valid_test = false; \
    } \
    return test_status;

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

typedef uint16_t CanTest_TestId;

/** This struct holds the results of attempting to run a given test */
typedef struct
{
    // This indicates if the test we tried to run is a valid test or not
    bool valid_test;
    // This indicates if the test we tried to run passed or not
    bool test_passed;
    // These 4 bytes are used to return custom values (if needed) from each test
    uint8_t extra_values[4];
} CanTest_TestResult;


/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief Attempt to run the test with the given ID
 * @param test_id The id of the test to run
 * @return A CanTest_TestResult indicating the result of running the given test
 */
CanTest_TestResult CanTest_runTestWithId(CanTest_TestId test_id);

// TODO: We _really_ should make this description a bit better
/**
 * @brief If the message given is a valid test message, attempts to run the 
 *        test that it specifies. If the test is invalid, passes, or fails, 
 *        it will broadcast a CAN message indicating as such. 
 * @param rx_msg a CAN message that may or may not be a test message
 */
void CanTest_handleCanMsg(CanRxMsg_Struct rx_msg);

#endif /* CAN_TEST_H */
