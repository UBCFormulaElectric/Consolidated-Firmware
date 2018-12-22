/**
 * @file  TODO: Name of this file goes here (ex. Template.h)
 * @brief TODO: A brief summary of what is contained in this file
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

// TODO: explanatory comment here
// TODO: We should be explicitly checking the test_id is valid `uint16_t`
#define CANTEST_TEST(TEST_ID) \ 
    void CanTest_Test_##TEST_ID(CanTest_TestStatus* test_status)

// TODO: explanatory comment here
#define CANTEST_ASSERT(COND) \
    if (!( COND )) { \
        test_status->test_passed = false; \
        return; \
    }

#define CANTEST_START_TESTS \
    CanTest_TestStatus test_status = \
    { \
        .valid_test = true, \
        .test_passed = true, \
        .test_results = { 0, 0, 0, 0 } \
    }; \
    switch(test_id) {

#define CANTEST_ADD_TEST(TEST_ID) \
        case TEST_ID: \
            CanTest_Test_##TEST_ID(&test_status); \
            break;

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

// TODO: Rename this to `CanTest_TestResult`?
/** This struct holds the results of attempting to run a given test */
typedef struct
{
    // This indicates if the test we tried to run is a valid test or not
    bool valid_test;
    // This indicates if the test we tried to run passed or not
    bool test_passed;
    // TODO: "test_results" is not a terribly good name.... need to indicate that they could be _anything_
    // These 4 bytes are used to return custom values from each test
    uint8_t test_results[4];
} CanTest_TestStatus;


/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief Attempt to run the test with the given ID
 * @param test_id The id of the test to run
 * @return TODO? Seems pretty self explanatory....
 */
CanTest_TestStatus CanTest_runTestWithId(CanTest_TestId test_id);

// TODO: We _really_ should make this description a bit better
/**
 * @brief If the message given is a valid test message, attempts to run the 
 *        test that it specifies. If the test is invalid, passes, or fails, 
 *        it will broadcast a CAN message indicating as such. 
 * @param rx_msg a CAN message that may or may not be a test message
 */
void CanTest_handleCanMsg(CanRxMsg_Struct rx_msg);

#endif /* CAN_TEST_H */
