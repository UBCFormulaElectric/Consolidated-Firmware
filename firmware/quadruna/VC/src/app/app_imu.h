#include "io_imu_config.h"
#include "io_imu.h"
#include "app_canTx.h"
#include <stdbool.h>

void app_collect_imu_data();

const ImuData *app_get_imu_struct();