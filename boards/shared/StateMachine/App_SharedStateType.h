#pragma once

#include <stdint.h>

// clang-format off
#define ADD_STATE(state_name, on_entry, on_exit, state_action) \
static const struct State* _next_state; \
static void SetNextState(struct State* state) \
{ \
    _next_state = state; \
} \
static const struct State* GetNextState(void) \
{ \
    return _next_state; \
} \
struct State prv_##state_name = \
{ \
    .run_on_entry     = on_entry,  \
    .run_on_exit      = on_exit,   \
    .run_state_action = state_action,  \
    .set_next_state   = SetNextState, \
    .get_next_state   = GetNextState, \
}; \
struct State* const state_name = &prv_##state_name \

struct State
{
    void (*run_on_entry)(void);
    void (*run_on_exit)(void);
    void (*run_state_action)(void);
    void (*set_next_state)(struct State*);
    const struct State*(*get_next_state)(void);
};
