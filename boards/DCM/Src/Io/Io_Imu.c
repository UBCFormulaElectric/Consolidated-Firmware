#include "Io_Imu.h"

struct Imu
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;
};

static struct Imu lsm_6ds33;
I2C_HandleTypeDef *i2c_hand;
uint8_t buf[RXBUFFERSIZE];
// extern struct SharedI2c *imu_i2c;
// struct SharedI2c *       imu_i2c = NULL;

float Io_Imu_GetAccelerationX()
{
    // Register LSB 0x28H and MSB 0x29H
    retrieveData(LSM6DSLTR_AccelX_LSB, LSM6DSLTR_AccelX_MSB);
    return lsm_6ds33.accel_x;
}

float Io_Imu_GetAccelerationY()
{
    // Register LSB 0x2AH and MSB 0x2BH
    retrieveData(LSM6DSLTR_AccelY_LSB, LSM6DSLTR_AccelY_MSB);
    return lsm_6ds33.accel_y;
}

float Io_Imu_GetAccelerationZ()
{
    // Register LSB 0x2CH and MSB 0x2DH
    retrieveData(LSM6DSLTR_AccelZ_LSB, LSM6DSLTR_AccelZ_MSB);
    return lsm_6ds33.accel_z;
}

void retrieveData(uint8_t regLSB, uint8_t regMSB) {
    // Start condition ST high to low 
    // // send
    int16_t val;
    float acceleration;

    do 
    {
        // start and then trasnmit then ack built in function
        if (HAL_I2C_Master_Transmit_DMA(i2c_hand, LSM6DSLTR_ADDR << 8, buf, 1) != HAL_OK) {
            Error_Handler();
        }

    // If no slave acknowledgement, retry communication once current state is ready
    } while (HAL_I2C_GetError(i2c_hand) == HAL_I2C_ERROR_AF && HAL_I2C_GetState(i2c_hand) == HAL_I2C_STATE_READY);

    
    do 
    {
        // start and then trasnmit then ack built in function
        if (HAL_I2C_Master_Transmit_DMA(i2c_hand, regMSB << 8 || regLSB, buf, 1) != HAL_OK) {
            Error_Handler();
        }
    // If no slave acknowledgement, retry communication once current state is ready
    } while (HAL_I2C_GetError(i2c_hand) == HAL_I2C_ERROR_AF && HAL_I2C_GetState(i2c_hand) == HAL_I2C_STATE_READY);


    //repeated start
    do 
    {
        // start and then trasnmit then ack built in function
        if (HAL_I2C_Master_Transmit_DMA(i2c_hand, LSM6DSLTR_ADDR << 8, buf, 1) != HAL_OK) {
            Error_Handler();
        }

    // If no slave acknowledgement, retry communication once current state is ready
    } while (HAL_I2C_GetError(i2c_hand) == HAL_I2C_ERROR_AF && HAL_I2C_GetState(i2c_hand) == HAL_I2C_STATE_READY);


    do 
    {
        if (HAL_I2C_Master_Receive_DMA(i2c_hand, (LSM6DSLTR_ADDR || 1) << 8, buf, 2) != HAL_OK) {
            Error_Handler();
        }


    // If no slave acknowledgement, retry communication once current state is ready
    } while (HAL_I2C_GetError(i2c_hand) == HAL_I2C_ERROR_AF && HAL_I2C_GetState(i2c_hand) == HAL_I2C_STATE_READY);


    val = ((int16_t)buf[0] << 4) | buf[1] >> 4;
    // acceleration = float(val);

    switch(regLSB) {
        case LSM6DSLTR_AccelX_LSB:
            lsm_6ds33.accel_x = (float)val;
            break;
        case LSM6DSLTR_AccelY_LSB:
            lsm_6ds33.accel_y = (float)val;
            break;
        case LSM6DSLTR_AccelZ_LSB:
            lsm_6ds33.accel_z = (float)val;
            break;
        default:
            Error_Handler();
    }
}

void Io_Imu_InitI2cHandle(I2C_HandleTypeDef *i2c_handle)
{
    assert(i2c_handle != NULL);

    i2c_hand = i2c_handle;

    // write to activate mode wanted

    // // Initialize the I2C interface to communicate with the LSM6DSLT
    // imu_i2c = Io_SharedI2c_Create(i2c_handle);

}