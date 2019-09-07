/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedI2C.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

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
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

HAL_StatusTypeDef SharedI2C_Init(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_Init(hi2c);
}

HAL_StatusTypeDef Shared_Master_Transmit_IT(
    I2C_HandleTypeDef *hi2c,
    uint16_t           DevAddress,
    uint8_t *          pData,
    uint16_t           Size)
{
    HAL_I2C_Master_Transmit_IT(hi2c, DevAddress, pData, Size);
}

HAL_StatusTypeDef SharedI2C_Master_Receive_IT(
    I2C_HandleTypeDef *hi2c,
    uint16_t           DevAddress,
    uint8_t *          pData,
    uint16_t           Size)
{
    HAL_I2C_Master_Receive_IT(hi2c, DevAddress, pData, Size);
}

HAL_StatusTypeDef SharedI2C_Slave_Transmit_IT(
    I2C_HandleTypeDef *hi2c,
    uint8_t *          pData,
    uint16_t           Size)
{
    HAL_I2C_Slave_Transmit_IT(hi2c, pData, Size);
}

HAL_StatusTypeDef SharedI2C_Slave_Receive_IT(
    I2C_HandleTypeDef *hi2c,
    uint8_t *          pData,
    uint16_t           Size)
{
    HAL_I2C_Slave_Receive_IT(hi2c, pData, Size);
}
