/**
 * @brief Shared library for wrapping HAL I2C library
 *
 * NOTE: Most of the functions here just directly call HAL functions, but we
 * have this shared library for two reasons: 1) To make any switch away from HAL
 * in the future a bit easier 2) To allow us to stub out methods so we can test
 * without actually having any I2C devices connected to the board.
 */

#ifndef SHARED_I2C_H
#define SHARED_I2C_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal_i2c.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
typedef struct
{
    uint16_t     pin;
    I2C_TypeDef *port;
} I2C_PinPort_Struct;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Initializes the I2C according to the specified parameters
 *         in the I2C_InitTypeDef and initialize the associated handle.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval HAL status
 */
HAL_StatusTypeDef SharedI2C_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief  Transmit in master mode an amount of data in non-blocking mode with
 * Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef Shared_Master_Transmit_IT(
    I2C_HandleTypeDef *hi2c,
    uint16_t           DevAddress,
    uint8_t *          pData,
    uint16_t           Size);

/**
 * @brief  Receive in master mode an amount of data in non-blocking mode with
 * Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  DevAddress Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef SharedI2C_Master_Receive_IT(
    I2C_HandleTypeDef *hi2c,
    uint16_t           DevAddress,
    uint8_t *          pData,
    uint16_t           Size);

/**
 * @brief  Transmit in slave mode an amount of data in non-blocking mode with
 * Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef SharedI2C_Slave_Transmit_IT(
    I2C_HandleTypeDef *hi2c,
    uint8_t *          pData,
    uint16_t           Size);

/**
 * @brief  Receive in slave mode an amount of data in non-blocking mode with
 * Interrupt
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @param  pData Pointer to data buffer
 * @param  Size Amount of data to be sent
 * @retval HAL status
 */
HAL_StatusTypeDef SharedI2C_Slave_Receive_IT(
    I2C_HandleTypeDef *hi2c,
    uint8_t *          pData,
    uint16_t           Size);

#endif /* SHARED_I2C_H */
