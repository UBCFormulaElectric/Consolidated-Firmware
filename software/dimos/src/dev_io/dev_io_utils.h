#pragma once

enum struct WaitDelegateResult
{
    TIMEOUT,
    INTERRUPTED
};

[[nodiscard]] WaitDelegateResult wait_delegate_thread();