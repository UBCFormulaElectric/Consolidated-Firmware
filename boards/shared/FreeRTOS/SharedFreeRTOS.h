/**
 * @brief A shared collection of FreeRTOS utilities
 */

#ifndef SHARED_FREERTOS_H
#define SHARED_FREERTOS_H
/******************************************************************************
 * Typedefs
 ******************************************************************************/
#include <FreeRTOS.h>

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** @brief Required data structures for creating a static FreeRTOS queue */
struct FreeRTOSStaticQueue
{
    StaticQueue_t state;
    uint8_t *     storage;
    QueueHandle_t handle;
};

#endif /* SHARED_FREERTOS_H */
