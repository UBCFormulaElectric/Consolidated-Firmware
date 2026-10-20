#pragma once

namespace io::switches
{
[[nodiscard]] bool start_get();
[[nodiscard]] bool telem_mark_get();
[[nodiscard]] bool launch_control_get();
[[nodiscard]] bool torque_vectoring_get();
[[nodiscard]] bool regen_get();
} // namespace io::switches
