#include <stm32f3xx_hal.h>

#include "main.h"
#include "Io_Imu_LSM6DS33.h"
#include "auto_generated/App_CanMsgsTx.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case IMU_INT_Pin:
            Io_Imu_LSM6DS33_readIMUData();
            ImuData imu_data;
            if (Io_Imu_LSM6DS33_getImuData(&imu_data))
            {
                App_CanMsgsTx_GetPeriodicCanTxPayloads()
                    ->dcm_imu_accel_x.acceleration = imu_data.accel_x;
                App_CanMsgsTx_GetPeriodicCanTxPayloads()
                    ->dcm_imu_accel_y.acceleration = imu_data.accel_y;
                App_CanMsgsTx_GetPeriodicCanTxPayloads()
                    ->dcm_imu_accel_z.acceleration = imu_data.accel_z;
            }
    }
}
