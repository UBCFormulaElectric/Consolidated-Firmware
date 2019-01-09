/**
 * @file  SharedUtilities.h
 * @brief
 */
#ifndef SHAREDUTILIT
#define TEMPLATE_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define EXAMPLE         32

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** TODO: What is this enum for */
typedef enum
{
    TEST_TEST1, /** OPTIONAL TODO: Test Type 1 */
    TEST_TEST2, /** OPTIONAL TODO: Test Type 2 */
} Test_Enum;

/** TODO: What is this struct for */
typedef struct
{
    TEST_TEST3,     /** OPTIONAL TODO: Test Type 3 */
        TEST_TEST4, /** OPTIONAL TODO: Test Type 4 */
} Test_Struct;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern uint32_t global_bar;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  This function adds two given numbers together
 * @param  num1 The first number to add
 * @param  num2 The second number to add
 * @return The sum of num1 and num2
 */
uint32_t addTwoNumbers(uint32_t num1, uint32_t num2);

// TODO: Update comment below to match `#ifndef` header guard at top of file
#endif /* TEMPLATE_H */
