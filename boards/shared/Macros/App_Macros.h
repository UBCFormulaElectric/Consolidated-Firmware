/**
 * @brief Common macros that may be shared between projects
 */
#ifndef APP_MACROS_H
#define APP_MACROS_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
// clang-format off
#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    sizeof(array_pointer) / sizeof(array_pointer[0])

#endif /* APP_MACROS_H */
