/******************************************************************************
 * Includes
 ******************************************************************************/
#include "CanTest.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
#ifdef PDM
    // TODO
#elif FSM
    // TODO
#elif BMS
    #define CANTEST_START_MSG_STDID     TEST_START_BMS_STDID
    #define CANTEST_RESULT_MSG_STDID    TEST_RESULT_BMS_STDID
    #define CANTEST_START_MSG_DLC       TEST_START_BMS_DLC
    #define CANTEST_RESULT_MSG_DLC      TEST_RESULT_BMS_DLC
#elif FSM
    // TODO
#elif DCM
    // TODO
#else
    #error "CanTest - No valid PCB name selected"
#endif

// For this library to work, we require that the start and result messsages
// have the same format
// TODO: We should specify the layout for the start and result messages (probably in the .sym file)
_Static_assert(CANTEST_START_MSG_DLC == 2, 
        "CanTest Start message data length != 2");
_Static_assert(CANTEST_RESULT_MSG_DLC == 8, 
        "CanTest Result message data length != 8");

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

// TODO: Should private functions like this be prefixed with `CanTest_`?
/**
 * @brief Convert the given TestStatus into a 8 element 8-byte array for 
 *        transmission over CAN
 * @param test_id The id of the test that the given results are for
 * @param test_result The TestStatus to convert into an array for a CAN message
 * @param test_result_msg An 8-element array in which the CAN representation of
 *                        the given test_result and test_id will be placed
 */
void TestResultToCanMsg(CanTest_TestId test_id, CanTest_TestResult test_result, uint8_t* test_result_msg);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

void TestResultToCanMsg(CanTest_TestId test_id, CanTest_TestResult test_result, uint8_t* test_result_msg){
    // Pack the booleans values into the zero'th byte
    test_result_msg[0] = test_result.valid_test + (test_result.test_passed << 1);

    // Use the first and second bytes to store the test ID
    // TODO: Is there a proper order (in terms of where the LSB shoud go, either 1 or 2)? Either way, should leave a more descriptive comment explaining this
    test_result_msg[1] = (test_id >> 8);
    test_result_msg[2] = (test_id & (0 << 8));

    // NOTE: Byte 3 is being saved here for potential future use

    // Use the last 4 bytes to store any extra data from the test
    test_result_msg[4] = test_result.extra_values[0];
    test_result_msg[5] = test_result.extra_values[1];
    test_result_msg[6] = test_result.extra_values[2];
    test_result_msg[7] = test_result.extra_values[3];
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

__weak CanTest_TestResult CanTest_runTest(uint16_t test_id)
{
    /* NOTE: This function Should not be modified, when the callback is needed,
              CanTest_runTest could be implemented in the CanTest.c file */
    CANTEST_START_TESTS;
    CANTEST_END_TESTS;
}

void CanTest_handleCanMsg(CanRxMsg_Struct rx_msg) {
    if (rx_msg.rx_header.StdId == CANTEST_START_MSG_STDID) {
        // TODO: We should _really_ document the structure for the CAN msg somewhere...
        
        // Construct the test_id from the CAN message
        // TODO: Should the order for the MSB and LSB be different? If so 
        // we should probably switch it in the result as well
        CanTest_TestId test_id = (rx_msg.data[0] << 8) + rx_msg.data[1];

        // Attempt to run the given test
        CanTest_TestResult test_result = CanTest_runTest(test_id);

        // Send the result of running the test over CAN
        uint8_t test_result_msg[8];
        TestResultToCanMsg(test_id, test_result, test_result_msg);
        SharedCan_TransmitDataCan(
                CANTEST_RESULT_MSG_STDID, 
                CANTEST_RESULT_MSG_DLC,
                test_result_msg
                );
    }
}
