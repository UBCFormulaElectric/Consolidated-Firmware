#include "math.hpp"
#include "app_segments.hpp"
#include "app_powerLimit.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_tractiveSystem.hpp"

#define MAX_DISCHARGE_POWER_LIMIT_W 78.0e3f
#define MAX_CHARGE_POWER_LIMIT_W 15.0e3f
#define MAX_DISCHARGE_CURRENT_LIMIT (- MAX_TS_DISCHARGE_CURRENT_AMPS)
#define MIN_DISCHARGE_CURRENT_LIMIT 10.0f
#define TEMP_DERATING_THRESHOLD 50.0f

namespace app::plim {

    

} // namespace app::plim