#pragma once

#include "hw_spi.h"
#include "hw_spis.h"

#include "util_errorCodes.hpp"

namespace io::imu
{
    class imu
    {
        private:
          const SpiDevice &imu_spi_handle;
        
        typedef struct
        {
            uint8_t x_h, x_l, y_h, y_l, z_h, z_l;
        } InertialDataRaw;

        typedef struct
        {
            uint8_t t_h, t_l;
        } TempRawData;

        using InertialDataRaw = AccelDataRaw;
        using InertialDataRaw = GyroDataRaw;

        typedef struct
        {
            float x, y, z;
        } ImuData;

        public:
          using ImuData = AccelData;
          using ImuData = GyroData;

          explicit imu(const SpiDevice &in_imu_spi_handle) : imu_spi_handle(in_imu_spi_handle) {}

          /**
           * @brief Get acceleration in the x acis
           * @param accel_x
           * 
           * @return ExitCode OK for success, otherwise fail
           */
          ExitCode getAccelX(float &accel_x);
          ExitCode getAccelY(float &accel_y);
          ExitCode getAccelZ(float &accel_z);

          ExitCode getGyroX(float &gyro_x);
          ExitCode getGyroY(float &gyro_y);
          ExitCode getGyroZ(float &gyro_z);

          ExitCode getTemp(float &temp);

          ExitCode getAccelAll(AccelData &data);
          ExitCode getGyroAll(GyroData &data);
    };
}