#pragma once

// Placeholder header for the segments module which has not yet been merged into the hexray BMS.
// Only the accessors used by other modules are forward-declared here. The test build provides
// stub implementations; the production build will not link until the real segments module lands.

namespace app::segments
{
float getMaxCellVoltage();
float getMaxCellTemp();
float getPackVoltage();
} // namespace app::segments
