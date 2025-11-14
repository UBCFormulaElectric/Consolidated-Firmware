#include "hw_error.h"
#include "hw_ubsan.h"

void __ubsan_handle_add_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_sub_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_mul_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_negate_overflow(void *_data, void *old_val);    // NOLINT(*-reserved-identifier)
