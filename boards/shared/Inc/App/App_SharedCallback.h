#pragma once

struct EventHandler
{
    void (*callback)(void *context);
    // The event handler does not take ownership of the context
    void *context;
};
