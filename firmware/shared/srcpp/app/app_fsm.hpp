#pragma once

#include "util_errorCodes.hpp"

namespace app
{
class Fsm
{
  public:
    struct State
    {
        const char *name;
        result<void> (*on_entry)();
        void (*on_tick)();
        result<void> (*on_exit)();
    };

    void init(const State &initial_state)
    {
        state      = &initial_state;
        next_state = &initial_state;
        entered    = false;
    }

    void setNextState(const State &s) { next_state = &s; }

    [[nodiscard]] const State *getCurrentState() const { return state; }

    void tick()
    {
        if (next_state != state)
        {
            entered = false;
            if (state->on_exit != nullptr && !state->on_exit().has_value())
                return;
            state = next_state;
        }

        if (!entered)
        {
            entered = state->on_entry == nullptr || state->on_entry().has_value();
            return;
        }

        if (state->on_tick != nullptr)
            state->on_tick();
    }

  private:
    const State *state      = nullptr;
    const State *next_state = nullptr;
    bool         entered    = false;
};
} // namespace app
