#pragma once

enum class WaitDelegateResult
{
    TIMEOUT,
    INTERRUPTED
};

[[nodiscard]] WaitDelegateResult wait_delegate_thread();