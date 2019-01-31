/**
 * @file  SharedMacros.h
 * @brief Shared helper macros
 */
#ifndef SHARED_MACROS_H
#define SHARED_MACROS_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
// clang-format off
#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    sizeof(array_pointer) / sizeof(array_pointer[0])

#define SECOND_TO_MILLSECOND(msecs) (uint32_t)(msecs * 1000)

#endif /* SHARED_MACROS_H */
