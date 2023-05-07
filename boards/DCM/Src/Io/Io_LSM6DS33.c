#include "Io_LSM6DS33.h"

struct LSM6DS33 {
  // Acceleration in m/s^2
  float accel_x;
  float accel_y;
  float accel_z;
};

static struct LSM6DS33 lsm_6ds33;

float Io_LSM6DS33_GetAccelerationX() { return lsm_6ds33.accel_x; }

float Io_LSM6DS33_GetAccelerationY() { return lsm_6ds33.accel_y; }

float Io_LSM6DS33_GetAccelerationZ() { return lsm_6ds33.accel_z; }
