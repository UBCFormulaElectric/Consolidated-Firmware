#include <cstdint>
#include "io_log.hpp"
#include "hw_error.hpp"
#include "hw_ubsan.hpp"

#include <algorithm>

extern "C"
{
    void __ubsan_handle_add_overflow(void *data, void *lhs, void *rhs);          // NOLINT(*-reserved-identifier)
    void __ubsan_handle_sub_overflow(void *data, void *lhs, void *rhs);          // NOLINT(*-reserved-identifier)
    void __ubsan_handle_mul_overflow(void *data, void *lhs, void *rhs);          // NOLINT(*-reserved-identifier)
    void __ubsan_handle_negate_overflow(void *_data, void *old_val);             // NOLINT(*-reserved-identifier)
    void __ubsan_handle_type_mismatch_v1(void *_data, void *ptr);                // NOLINT(*-reserved-identifier)
    void __ubsan_handle_out_of_bounds(void *_data, void *index);                 // NOLINT(*-reserved-identifier)
    void __ubsan_handle_shift_out_of_bounds(void *_data, void *lhs, void *rhs);  // NOLINT(*-reserved-identifier)
    void __ubsan_handle_load_invalid_value(void *_data, void *val);              // NOLINT(*-reserved-identifier)
    void __ubsan_handle_nonnull_arg(void *_data);                                // NOLINT(*-reserved-identifier)
    void __ubsan_handle_vla_bound_not_positive(void *_data, void *bound);        // NOLINT(*-reserved-identifier)
    void __ubsan_handle_pointer_overflow(void *_data, void *base, void *result); // NOLINT(*-reserved-identifier)
    void __ubsan_handle_divrem_overflow(void *_data, void *lhs, void *rhs);      // NOLINT(*-reserved-identifier)
} // extern "C"

#define VALUE_LENGTH 40

static int test_and_set_bit(const int bit, unsigned long *addr)
{
    const unsigned long mask = 1UL << bit;
    const int           old  = (*addr & mask) != 0; // Test old value
    *addr |= mask;                                  // Set the bit
    return old;
}
static bool suppress_report(source_location *location)
{
#define REPORTED_BIT 31
    return test_and_set_bit(REPORTED_BIT, &location->reported);
}
static bool type_is_int(const type_descriptor *type)
{
    return type->type_kind == type_kind_int;
}
static bool type_is_signed(const type_descriptor *type)
{
    if (!type_is_int(type))
        LOG_WARN("type_is_signed called with a non-int type");
    return type->type_info & 1;
}
static unsigned type_bit_width(const type_descriptor *type)
{
    return 1 << (type->type_info >> 1);
}
static bool is_inline_int(const type_descriptor *type)
{
    constexpr unsigned inline_bits = sizeof(unsigned long) * 8;
    const unsigned     bits        = type_bit_width(type);
    if (!type_is_int(type))
        LOG_WARN("is_inline_int called with a non-int type");

    return bits <= inline_bits;
}
static int64_t get_signed_val(const type_descriptor *type, void *val)
{
    if (is_inline_int(type))
    {
        const unsigned extra_bits = sizeof(int64_t) * 8 - type_bit_width(type);
        const auto     ulong_val  = reinterpret_cast<unsigned long>(val);

        return static_cast<int64_t>(ulong_val) << extra_bits >> extra_bits;
    }

    if (type_bit_width(type) == 64)
        return *static_cast<int64_t *>(val);

    return *static_cast<int64_t *>(val);
}
static uint64_t get_unsigned_val(const type_descriptor *type, void *val)
{
    if (is_inline_int(type))
        return reinterpret_cast<unsigned long>(val);

    if (type_bit_width(type) == 64)
        return *static_cast<uint64_t *>(val);

    return *static_cast<uint64_t *>(val);
}
static bool val_is_negative(const type_descriptor *type, void *val)
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

    buffer[std::min(i, size - 1U)] = '\0'; // Null-terminate the string
}

static void i64_to_str(const size_t size, char *buffer, const int64_t value)
{
    if (size < 2)
        return;
    uint64_t uvalue;
    // Handle negative values
    if (value < 0)
    {
        buffer[0] = '-';
        uvalue    = static_cast<uint64_t>(-(value + 1)) + 1; // Carefully negate to avoid overflow at INT64_MIN
    }
    else
    {
        uvalue = static_cast<uint64_t>(value);
    }
    const size_t offset = value < 0 ? 1 : 0;
    u64_to_str(size - offset, buffer + offset, uvalue);
}
static void val_to_string(char *str, const size_t size, const type_descriptor *type, void *value)
{
    if (!type_is_int(type))
    {
        return;
    }

    if (type_bit_width(type) == 128)
    {
        LOG_WARN("128 bit integers not supported");
    }
    else if (type_is_signed(type))
    {
        i64_to_str(size, str, get_signed_val(type, value));
    }
    else
    {
        u64_to_str(size, str, get_unsigned_val(type, value));
    }
}

/**
 *UBSAN UTILITIES
 */
#define COLUMN_MASK (~0U)
#define LINE_MASK (~(1U << REPORTED_BIT))
static void ubsan_prologue(const source_location *loc, const char *reason)
{
    LOG_ERROR(
        "UBSAN: %s in %s:%d:%d", reason, loc->file_name, loc->row_col.line & LINE_MASK,
        loc->row_col.column & COLUMN_MASK);
}

/**
 *  GENERALIZED HANDLERS
 */
static void handle_overflow(overflow_data *data, void *lhs, void *rhs, const char op)
{
    if (suppress_report(&data->location))
    {
        return;
    }

    type_descriptor *type = data->type;

    char lhs_val_str[VALUE_LENGTH];
    char rhs_val_str[VALUE_LENGTH];
    val_to_string(lhs_val_str, sizeof(lhs_val_str), type, lhs);
    val_to_string(rhs_val_str, sizeof(rhs_val_str), type, rhs);

    const char *reason = type_is_signed(type) ? "signed-integer-overflow" : "unsigned-integer-overflow";
    ubsan_prologue(&data->location, reason);
    LOG_ERROR("%s %c %s cannot be represented in type %s", lhs_val_str, op, rhs_val_str, type->type_name);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), reason);
}
/**
 * HANDLING POINTER BUGS
 */
static constexpr const char *const type_check_kinds[] = { "load of",
                                                          "store to",
                                                          "reference binding to",
                                                          "member access within",
                                                          "member call on",
                                                          "constructor call on",
                                                          "downcast of",
                                                          "downcast of" };

static void handle_null_ptr_deref(const type_mismatch_data_common *data)
{
    if (suppress_report(data->location))
        return;
    ubsan_prologue(data->location, "null-ptr-deref");
    LOG_ERROR("%s null pointer of type %s", type_check_kinds[data->type_check_kind], data->type->type_name);
    hw_error(data->location->file_name, static_cast<int>(data->location->row_col.line), "null-ptr-deref");
}
static void handle_misaligned_access(const type_mismatch_data_common *data, const unsigned long ptr)
{
    if (suppress_report(data->location))
        return;
    ubsan_prologue(data->location, "misaligned-access");
    LOG_ERROR(
        "%s misaligned address %p for type %s", type_check_kinds[data->type_check_kind], (void *)ptr,
        data->type->type_name);
    LOG_ERROR("which requires %ld byte alignment", data->alignment);
    hw_error(data->location->file_name, static_cast<int>(data->location->row_col.line), "misaligned-access");
}
static void handle_object_size_mismatch(const type_mismatch_data_common *data, const unsigned long ptr)
{
    if (suppress_report(data->location))
        return;
    ubsan_prologue(data->location, "object-size-mismatch");
    LOG_ERROR("%s address %p with insufficient space", type_check_kinds[data->type_check_kind], (void *)ptr);
    LOG_ERROR("for an object of type %s", data->type->type_name);
    hw_error(data->location->file_name, static_cast<int>(data->location->row_col.line), "object-size-mismatch");
}
static void ubsan_type_mismatch_common(const type_mismatch_data_common *data, unsigned long ptr)
{
#define IS_ALIGNED(x, a) (((x) & ((typeof(x))(a)-1)) == 0)
    if (!ptr)
    {
        handle_null_ptr_deref(data);
    }
    else if (data->alignment && !IS_ALIGNED(ptr, data->alignment))
    {
        handle_misaligned_access(data, ptr);
    }
    else
    {
        handle_object_size_mismatch(data, ptr);
    }
}
/**
 * Location???
 */
static bool location_is_valid(const source_location *loc)
{
    return loc->file_name != nullptr;
}
static void print_source_location(const char *prefix, const source_location *loc)
{
    LOG_ERROR("%s %s:%d:%d", prefix, loc->file_name, loc->row_col.line & LINE_MASK, loc->row_col.column & COLUMN_MASK);
}
// NOLINTBEGIN(bugprone-reserved-identifier)
#define REGISTER_OVERFLOW_HANDLER(handler_name, op)                        \
    void handler_name(void *data, void *lhs, void *rhs)                    \
    {                                                                      \
        handle_overflow(static_cast<overflow_data *>(data), lhs, rhs, op); \
    }
REGISTER_OVERFLOW_HANDLER(__ubsan_handle_add_overflow, '+')
REGISTER_OVERFLOW_HANDLER(__ubsan_handle_sub_overflow, '-')
REGISTER_OVERFLOW_HANDLER(__ubsan_handle_mul_overflow, '*')
void __ubsan_handle_negate_overflow(void *_data, void *old_val)
{
    auto *data = static_cast<overflow_data *>(_data);
    if (suppress_report(&data->location))
    {
        return;
    }

    char old_val_str[VALUE_LENGTH];
    ubsan_prologue(&data->location, "negation-overflow");
    val_to_string(old_val_str, sizeof(old_val_str), data->type, old_val);
    LOG_ERROR("negation of %s cannot be represented in type %s", old_val_str, data->type->type_name);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "negation-overflow");
}
void __ubsan_handle_divrem_overflow(void *_data, void *lhs, void *rhs)
{
    const auto data = static_cast<overflow_data *>(_data);
    char       lhs_val_str[VALUE_LENGTH];
    if (suppress_report(&data->location))
    {
        return;
    }
    ubsan_prologue(&data->location, "division-overflow");
    val_to_string(lhs_val_str, sizeof(lhs_val_str), data->type, lhs);
    if (type_is_signed(data->type) && get_signed_val(data->type, rhs) == -1)
        LOG_ERROR("division of %s by -1 cannot be represented in type %s\n", lhs_val_str, data->type->type_name);
    else
        LOG_ERROR("division by zero\n");
    hw_error(data->location.file_name, 0, "divrem-overflow");
}

// void __ubsan_handle_implicit_conversion(void *_data, void *lhs, void *rhs) {}
// void __ubsan_handle_type_mismatch(struct type_mismatch_data *data, void *ptr) {}
void __ubsan_handle_type_mismatch_v1(void *_data, void *ptr)
{
    auto                           *data = static_cast<type_mismatch_data_v1 *>(_data);
    const type_mismatch_data_common common_data{ &data->location, data->type, 1UL << data->log_alignment,
                                                 data->type_check_kind };
    ubsan_type_mismatch_common(&common_data, reinterpret_cast<unsigned long>(ptr));
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "type-mismatch");
}
void __ubsan_handle_out_of_bounds(void *_data, void *index)
{
    auto *data = static_cast<out_of_bounds_data *>(_data);
    if (suppress_report(&data->location))
    {
        return;
    }
    char index_str[VALUE_LENGTH];
    val_to_string(index_str, sizeof(index_str), data->index_type, index);
    ubsan_prologue(&data->location, "array-index-out-of-bounds");
    LOG_ERROR("index %s is out of range for type %s", index_str, data->array_type->type_name);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "array-index-out-of-bounds");
}
void __ubsan_handle_shift_out_of_bounds(void *_data, void *lhs, void *rhs)
{
    auto                  *data     = static_cast<shift_out_of_bounds_data *>(_data);
    const type_descriptor *rhs_type = data->rhs_type;
    type_descriptor       *lhs_type = data->lhs_type;

    if (suppress_report(&data->location))
        return;

    char rhs_str[VALUE_LENGTH];
    val_to_string(rhs_str, sizeof(rhs_str), rhs_type, rhs);
    char lhs_str[VALUE_LENGTH];
    val_to_string(lhs_str, sizeof(lhs_str), lhs_type, lhs);

    ubsan_prologue(&data->location, "shift-out-of-bounds");
    if (val_is_negative(rhs_type, rhs))
    {
        LOG_ERROR("shift exponent %s is negative", rhs_str);
    }
    else if (get_unsigned_val(rhs_type, rhs) >= type_bit_width(lhs_type))
    {
        LOG_ERROR(
            "shift exponent %s is too large for %u-bit type %s", rhs_str, type_bit_width(lhs_type),
            lhs_type->type_name);
    }
    else if (val_is_negative(lhs_type, lhs))
    {
        LOG_ERROR("left shift of negative value %s", lhs_str);
    }
    else
    {
        LOG_ERROR(
            "left shift of %s by %s places cannot be represented in type %s", lhs_str, rhs_str, lhs_type->type_name);
    }
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "shift-out-of-bounds");
}
// void __ubsan_handle_builtin_unreachable(void *_data);
void __ubsan_handle_load_invalid_value(void *_data, void *val)
{
    auto *data = static_cast<invalid_value_data *>(_data);
    if (suppress_report(&data->location))
        return;
    ubsan_prologue(&data->location, "invalid-load");
    char val_str[VALUE_LENGTH];
    val_to_string(val_str, sizeof(val_str), data->type, val);
    LOG_ERROR("load of value %s is not a valid value for type %s\n", val_str, data->type->type_name);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "invalid-load");
}
// void __ubsan_handle_alignment_assumption(void *_data, unsigned long ptr, unsigned long align, unsigned long offset)
// {}

/**
 * Not handled by linux
 */

/**
 * https://coral.googlesource.com/linux-imx/+/refs/heads/imx_4.14.98_2.0.0_ga/lib/ubsan.c
 */
void __ubsan_handle_nonnull_arg(void *_data)
{
    auto *data = static_cast<nonnull_arg_data *>(_data);

    if (suppress_report(&data->location))
    {
        return;
    }

    ubsan_prologue(&data->location, "nonnull-args");
    LOG_ERROR("null pointer passed as argument %d, declared with nonnull attribute\n", data->arg_index);
    if (location_is_valid(&data->attr_location))
        print_source_location("nonnull attribute declared in ", &data->attr_location);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "nonnull-args");
}
/**
 * https://android.googlesource.com/kernel/msm/+/android-7.1.0_r0.2/lib/ubsan.c
 */
void __ubsan_handle_vla_bound_not_positive(void *_data, void *bound)
{
    auto *data = static_cast<vla_bound_data *>(_data);
    if (suppress_report(&data->location))
    {
        return;
    }

    ubsan_prologue(&data->location, "vla-bound-not-positive");
    char bound_str[VALUE_LENGTH];
    val_to_string(bound_str, sizeof(bound_str), data->type, bound);
    LOG_ERROR("variable length array bound value %s <= 0\n", bound_str);
    hw_error(data->location.file_name, static_cast<int>(data->location.row_col.line), "vla-bound-not-positive");
}

void __ubsan_handle_pointer_overflow(void *_data, void *base, void *result)
{
    auto *data = static_cast<pointer_overflow_data *>(_data);
    if (suppress_report(&data->location))
        return;
    // struct source_location Loc = data->location;

    ubsan_prologue(&data->location, "pointer-overflow");
    if (base == nullptr && result == nullptr)
    {
        LOG_ERROR("applying zero offset to null pointer");
    }
    else if (base == nullptr && result != nullptr)
    {
        LOG_ERROR("applying non-zero offset %X to null pointer", result);
    }
    else if (base != nullptr && result == nullptr)
    {
        LOG_ERROR("applying non-zero offset to non-null pointer %X produced null pointer", base);
    }
    else if ((base != nullptr) == (result != nullptr))
    {
        if (base > result)
        {
            LOG_ERROR("addition of unsigned offset to %X overflowed to %X", base, result);
        }
        else
        {
            LOG_ERROR("subtraction of unsigned offset from %X overflowed to %X", base, result);
        }
    }
    else
    {
        LOG_ERROR("pointer index expression with base %X overflowed to %X", base, result);
    }
    hw_error(data->location.file_name, static_cast<int>((data)->location.row_col.line), "pointer-overflow");
}
// NOLINTEND(bugprone-reserved-identifier)