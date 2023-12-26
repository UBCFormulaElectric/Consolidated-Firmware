#pragma once

namespace can_handlers
{
[[noreturn]] void CanRXTask();
[[noreturn]] void CanPeriodicTXTask();
} // namespace can_handlers