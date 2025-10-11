#include "io_imu_config.h"
#include "io_imu.h"
#include "app_canTx.h"
#include <stdbool.h>

void app_imu_init();

void app_imu_broadcast();

const ImuData *app_imu_getData();

const ImuFaults *app_imu_getFaultData();