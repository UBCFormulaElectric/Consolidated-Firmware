#include <cstdint>
#include <cstddef>
extern "C" {
#include "hw_error.h"
#include "hw_ubsan.h"
#include "io_log.h"
#include "app_utils.h"

void __ubsan_handle_add_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_sub_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_mul_overflow(void *data, void *lhs, void *rhs); // NOLINT(*-reserved-identifier)
void __ubsan_handle_negate_overflow(void *_data, void *old_val);    // NOLINT(*-reserved-identifier)
[[noreturn]] void __ubsan_handle_type_mismatch_v1(void *_data, void *ptr);          // NOLINT(*-reserved-identifier)
void          __ubsan_handle_out_of_bounds(void *_data, void *index);               // NOLINT(*-reserved-identifier)
void          __ubsan_handle_shift_out_of_bounds(void *_data, void *lhs, void *rhs);// NOLINT(*-reserved-identifier)
void __ubsan_handle_load_invalid_value(void *_data, void *val);                     // NOLINT(*-reserved-identifier)
void __ubsan_handle_nonnull_arg(void *_data);                                // NOLINT(*-reserved-identifier)
void __ubsan_handle_vla_bound_not_positive(void *_data, void *bound);        // NOLINT(*-reserved-identifier)
void __ubsan_handle_pointer_overflow(void *_data, void *base, void *result); // NOLINT(*-reserved-identifier)
} // externC end

#define VALUE_LENGTH 40 
static int test_and_set_bit(const int bit, unsigned long *addr){
        const unsigned long mask = 1UL << bit;
    const int           old  = (*addr & mask) != 0; // Test old value
    *addr |= mask;                                  // Set the bit
    return old;
}
static bool suppress_report(struct source_location *location)
{
#define REPORTED_BIT 31
    return test_and_set_bit(REPORTED_BIT, &location->reported);
}
static bool type_is_int(const struct type_descriptor *type)
{
    return type->type_kind == type_kind_int;
}
static bool type_is_signed(const struct type_descriptor *type)
{
    if (!type_is_int(type))
        LOG_WARN("type_is_signed called with a non-int type");
    return type->type_info & 1;
}
static unsigned type_bit_width(const struct type_descriptor *type)
{
    return 1 << (type->type_info >> 1);
}
static bool is_inline_int(const struct type_descriptor *type)
{
    const unsigned inline_bits = sizeof(unsigned long) * 8;
    const unsigned bits        = type_bit_width(type);
    if (!type_is_int(type))
        LOG_WARN("is_inline_int called with a non-int type");

    return bits <= inline_bits;
}
static int64_t get_signed_val(const struct type_descriptor *type, void *val)
{
    if (is_inline_int(type))
    {
        const unsigned      extra_bits = sizeof(int64_t) * 8 - type_bit_width(type);
        const unsigned long ulong_val  = (unsigned long)val;

        return ((int64_t)ulong_val) << extra_bits >> extra_bits;
    }

    if (type_bit_width(type) == 64)
        return *(int64_t *)val;

    return *(int64_t *)val;
}
static uint64_t get_unsigned_val(const struct type_descriptor *type, void *val)
{
    if (is_inline_int(type))
        return (unsigned long)val;

    if (type_bit_width(type) == 64)
        return *(uint64_t *)val;

    return *(uint64_t *)val;
}
static bool val_is_negative(const struct type_descriptor *type, void *val)
{
    return type_is_signed(type) && get_signed_val(type, val) < 0;
}

// https://www.eevblog.com/forum/microcontrollers/printf-uint64_t-with-arm-gcc/
// note: i had to roll my own because 32 bit snprintf does not support 64 bit numbers
static void u64_to_str(const size_t size, char *buffer, uint64_t value)
{
    if (size < 2U)
        return;
    char   temp[21]; // Max length for uint64_t in base 10 is 20 digits + null terminator
    size_t i = 0U;
    // Special case for 0
    if (value == 0U)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    while (value > 0U)
    {
        temp[i++] = static_cast<char>(value % 10U) + '0'; // Get least significant digit
        value /= 10U;
    }

    // Reverse the digits into the buffer
    for (size_t j = 0U; j < i && j < size - 1U; j++)
    {
        buffer[j] = temp[i - j - 1U];
    }

    buffer[MIN(i, size - 1U)] = '\0'; // Null-terminate the string
}

