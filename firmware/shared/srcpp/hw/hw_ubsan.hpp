/**
 * This file has been shamelessly ripped from the linux standard library
 * https://github.com/torvalds/linux/blob/master/lib/ubsan.c
 */
#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
    /*
     * ABI defined by Clang's UBSAN enum SanitizerHandler:
     * https://github.com/llvm/llvm-project/blob/release/16.x/clang/lib/CodeGen/CodeGenFunction.h#L113
     * This one is far more readable: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
     */
    enum ubsan_checks
    {
        ubsan_add_overflow,
        ubsan_builtin_unreachable,
        ubsan_cfi_check_fail,
        ubsan_divrem_overflow,
        ubsan_dynamic_type_cache_miss,
        ubsan_float_cast_overflow,
        ubsan_function_type_mismatch,
        ubsan_implicit_conversion,
        ubsan_invalid_builtin,
        ubsan_invalid_objc_cast,
        ubsan_load_invalid_value,
        ubsan_missing_return,
        ubsan_mul_overflow,
        ubsan_negate_overflow,
        ubsan_nullability_arg,
        ubsan_nullability_return,
        ubsan_nonnull_arg,
        ubsan_nonnull_return,
        ubsan_out_of_bounds,
        ubsan_pointer_overflow,
        ubsan_shift_out_of_bounds,
        ubsan_sub_overflow,
        ubsan_type_mismatch,
        ubsan_alignment_assumption,
        ubsan_vla_bound_not_positive,
    };

    enum
    {
        /// An integer type. Lowest bit is 1 for a signed value, 0 for an unsigned
        /// value. Remaining bits are log_2(bit width). The value representation is
        /// the integer itself if it fits into a ValueHandle, and a pointer to the
        /// integer otherwise.
        type_kind_int = 0x0000,
        /// A floating-point type. Low 16 bits are bit width. The value
        /// representation is that of bitcasting the floating-point value to an
        /// integer type.
        type_kind_float = 0x0001,
        /// An _BitInt(N) type. Lowest bit is 1 for a signed value, 0 for an
        /// unsigned value. Remaining bits are log_2(bit_width). The value
        /// representation is the integer itself if it fits into a ValueHandle, and
        /// a pointer to the integer otherwise. TypeName contains the true width
        /// of the type for the signed _BitInt(N) type stored after zero bit after
        /// TypeName as 32-bit unsigned integer.
        TK_BitInt = 0x0002,
        /// Any other type. The value representation is unspecified.
        type_unknown = 0xffff
    };

    struct type_descriptor
    {
        uint16_t type_kind;
        uint16_t type_info;
        char     type_name[1];
    };

    struct source_location
    {
        const char *file_name;
        union
        {
            unsigned long reported;
            struct
            {
                uint32_t line;
                uint32_t column;
            } row_col;
        };
    };

    struct overflow_data
    {
        struct source_location  location;
        struct type_descriptor *type;
    };

    struct implicit_conversion_data
    {
        struct source_location  location;
        struct type_descriptor *from_type;
        struct type_descriptor *to_type;
        unsigned char           type_check_kind;
    };

    struct type_mismatch_data
    {
        struct source_location  location;
        struct type_descriptor *type;
        unsigned long           alignment;
        unsigned char           type_check_kind;
    };

    struct type_mismatch_data_v1
    {
        struct source_location  location;
        struct type_descriptor *type;
        unsigned char           log_alignment;
        unsigned char           type_check_kind;
    };

    struct type_mismatch_data_common
    {
        struct source_location *location;
        struct type_descriptor *type;
        unsigned long           alignment;
        unsigned char           type_check_kind;
    };

    struct nonnull_arg_data
    {
        struct source_location location;
        struct source_location attr_location;
        int                    arg_index;
    };

    struct out_of_bounds_data
    {
        struct source_location  location;
        struct type_descriptor *array_type;
        struct type_descriptor *index_type;
    };

    struct shift_out_of_bounds_data
    {
        struct source_location  location;
        struct type_descriptor *lhs_type;
        struct type_descriptor *rhs_type;
    };

    struct unreachable_data
    {
        struct source_location location;
    };

    struct invalid_value_data
    {
        struct source_location  location;
        struct type_descriptor *type;
    };

    struct alignment_assumption_data
    {
        struct source_location  location;
        struct source_location  assumption_location;
        struct type_descriptor *type;
    };

    // also stole from https://github.com/espressif/esp-idf/blob/master/components/esp_system/ubsan.c
    struct vla_bound_data
    {
        struct source_location  location;
        struct type_descriptor *type;
    };

    struct pointer_overflow_data
    {
        struct source_location location;
    };

#ifdef __cplusplus
}
#endif
