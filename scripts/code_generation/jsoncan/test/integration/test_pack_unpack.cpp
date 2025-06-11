#include <gtest/gtest.h>

extern "C"
{
#include "app_canUtils.h"
}

#define LSHIFT(value, shift) ((uint64_t)value << shift)
#define ENCODE(input, scale, offset) (((input - offset) / scale))
#define TWOS_COMP(input, max) (0xFFFF - input + 1U)

TEST(PackUnpackTests, test_basic_signal_types)
{
    const ECU1_BasicSignalTypes_Signals in_msgs[3] = {
        // All zeros.
        { .ECU1_Boolean1_value = false,
          .ECU1_Boolean2_value = false,
          .ECU1_Enum_value     = ENUM_EX_0,
          .ECU1_UInt8_value    = 0,
          .ECU1_UInt16_value   = 0,
          .ECU1_UInt32_value   = 0 },
        // (Mostly) all ones.
        { .ECU1_Boolean1_value = 1,
          .ECU1_Boolean2_value = 1,
          .ECU1_Enum_value     = ENUM_EX_2,
          .ECU1_UInt8_value    = 0xFF,
          .ECU1_UInt16_value   = 0xFFFF,
          .ECU1_UInt32_value   = 0xFFFFFFFF },
        // Some random signal values.
        { .ECU1_Boolean1_value = true,
          .ECU1_Boolean2_value = false,
          .ECU1_Enum_value     = ENUM_EX_1,
          .ECU1_UInt8_value    = 0xFE,
          .ECU1_UInt16_value   = 123,
          .ECU1_UInt32_value   = 0xFAFAFAFA },
    };
    const uint64_t expected_payloads[3] = {
        0,
        0 | LSHIFT(true, 0) |       // Boolean1
            LSHIFT(true, 1) |       // Boolean2
            LSHIFT(ENUM_EX_2, 2) |  // Enum
            LSHIFT(0xFF, 4) |       // Uint8
            LSHIFT(0xFFFF, 12) |    // Uint16
            LSHIFT(0xFFFFFFFF, 28), // Uint32
        0 | LSHIFT(true, 0) |       // Boolean1
            LSHIFT(false, 1) |      // Boolean2
            LSHIFT(ENUM_EX_1, 2) |  // Enum
            LSHIFT(0xFE, 4) |       // Uint8
            LSHIFT(123, 12) |       // Uint16
            LSHIFT(0xFAFAFAFA, 28), // Uint32
    };

    for (int i = 0; i < 3; i++)
    {
        uint8_t payload[8] = { 0 };
        app_canUtils_ECU1_BasicSignalTypes_pack(&in_msgs[i], payload);

        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *(uint64_t *)payload);

        // Confirm we can decode the payload, which should match the original message.
        ECU1_BasicSignalTypes_Signals out_msg;
        app_canUtils_ECU1_BasicSignalTypes_unpack(payload, &out_msg);
        ASSERT_EQ(in_msgs[i].ECU1_Boolean1_value, out_msg.ECU1_Boolean1_value);
        ASSERT_EQ(in_msgs[i].ECU1_Boolean2_value, out_msg.ECU1_Boolean2_value);
        ASSERT_EQ(in_msgs[i].ECU1_Enum_value, out_msg.ECU1_Enum_value);
        ASSERT_EQ(in_msgs[i].ECU1_UInt8_value, out_msg.ECU1_UInt8_value);
        ASSERT_EQ(in_msgs[i].ECU1_UInt16_value, out_msg.ECU1_UInt16_value);
        ASSERT_EQ(in_msgs[i].ECU1_UInt32_value, out_msg.ECU1_UInt32_value);
    }
}

TEST(PackUnpackTests, test_decimal_numbers)
{
    const ECU1_DecimalNumbers_Signals in_msgs[5] = {
        { .ECU1_Decimal1_value = -100.0f, .ECU1_Decimal2_value = 2.0f },
        { .ECU1_Decimal1_value = -54.61f, .ECU1_Decimal2_value = 2.1243f },
        { .ECU1_Decimal1_value = 0.0f, .ECU1_Decimal2_value = 2.5f },
        { .ECU1_Decimal1_value = 72.34, .ECU1_Decimal2_value = 2.784f },
        { .ECU1_Decimal1_value = 100.0f, .ECU1_Decimal2_value = 3.0f },
    };
    const uint64_t expected_payloads[5] = {
        0 | LSHIFT(ENCODE(-100.0f, 0.1f, -100.0f), 0) | // Decimal1
            LSHIFT(ENCODE(2.0f, 0.01f, 2.0f), 11),      // Decimal2
        0 | LSHIFT(ENCODE(-54.61f, 0.1f, -100.0f), 0) | // Decimal1
            LSHIFT(ENCODE(2.1243f, 0.01f, 2.0f), 11),   // Decimal2
        0 | LSHIFT(ENCODE(0.0f, 0.1f, -100.0f), 0) |    // Decimal1
            LSHIFT(ENCODE(2.5f, 0.01f, 2.0f), 11),      // Decimal2
        0 | LSHIFT(ENCODE(72.34f, 0.1f, -100.0f), 0) |  // Decimal1
            LSHIFT(ENCODE(2.781f, 0.01f, 2.0f), 11),    // Decimal2
        0 | LSHIFT(ENCODE(100.0f, 0.1f, -100.0f), 0) |  // Decimal1
            LSHIFT(ENCODE(3.0f, 0.01f, 2.0f), 11),      // Decimal2
    };
    const ECU1_DecimalNumbers_Signals expected_unpacked[5] = {
        { .ECU1_Decimal1_value = -100.0f, .ECU1_Decimal2_value = 2.0f },
        { .ECU1_Decimal1_value = -54.7f, .ECU1_Decimal2_value = 2.12f },
        { .ECU1_Decimal1_value = 0.0f, .ECU1_Decimal2_value = 2.5f },
        { .ECU1_Decimal1_value = 72.3, .ECU1_Decimal2_value = 2.78f },
        { .ECU1_Decimal1_value = 100.0f, .ECU1_Decimal2_value = 3.0f },
    };

    for (int i = 0; i < 5; i++)
    {
        uint8_t payload[8] = { 0 };
        app_canUtils_ECU1_DecimalNumbers_pack(&in_msgs[i], payload);

        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *(uint64_t *)payload);

        // Confirm we can decode the payload, which should match the original message.
        ECU1_DecimalNumbers_Signals out_msg;
        app_canUtils_ECU1_DecimalNumbers_unpack(payload, &out_msg);
        ASSERT_NEAR(out_msg.ECU1_Decimal1_value, expected_unpacked[i].ECU1_Decimal1_value, 1e-5);
        ASSERT_NEAR(out_msg.ECU1_Decimal2_value, expected_unpacked[i].ECU1_Decimal2_value, 1e-6);
    }
}

TEST(PackUnpackTests, test_signed_numbers)
{
    const ECU1_DbcMatching_Signals in_msgs[5] = {
        { .ECU1_DbcMatchingTemp_value = 0.0f, .ECU1_DbcMatchingRpm_value = 0 },
        {
            .ECU1_DbcMatchingTemp_value = 1.0f,
            .ECU1_DbcMatchingRpm_value  = 1,
        },
        {
            .ECU1_DbcMatchingTemp_value = -1.0f,
            .ECU1_DbcMatchingRpm_value  = -1,
        },
        {
            .ECU1_DbcMatchingTemp_value = 3276.7f,
            .ECU1_DbcMatchingRpm_value  = 32767,
        },
        {
            .ECU1_DbcMatchingTemp_value = -3276.8f,
            .ECU1_DbcMatchingRpm_value  = -32768,
        },
    };
    const uint64_t expected_payloads[5] = {
        0,
        0 | LSHIFT(ENCODE(1.0, 0.1, 0), 0) |                        // Temp
            LSHIFT(ENCODE(1, 1, 0), 32),                            // RPM
        0 | LSHIFT(TWOS_COMP(ENCODE(1.0, 0.1, 0), 0xFFFF), 0) |     // Temp
            LSHIFT(TWOS_COMP(ENCODE(1, 1, 0), 0xFFFF), 32),         // RPM
        0 | LSHIFT(ENCODE(3276.7f, 0.1, 0), 0) |                    // Temp
            LSHIFT(ENCODE(32767, 1, 0), 32),                        // RPM
        0 | LSHIFT(TWOS_COMP(ENCODE(3276.8, 0.1, 0), 0xFFFF), 32) | // Temp
            LSHIFT(TWOS_COMP(ENCODE(32768, 1, 0), 0xFFFF), 0),      // RPM
    };

    for (int i = 0; i < 4; i++)
    {
        uint8_t payload[8] = { 0 };
        app_canUtils_ECU1_DbcMatching_pack(&in_msgs[i], payload);

        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *(uint64_t *)payload);

        // Confirm we can decode the payload, which should match the original message.
        ECU1_DbcMatching_Signals out_msg;
        app_canUtils_ECU1_DbcMatching_unpack(payload, &out_msg);
        ASSERT_NEAR(out_msg.ECU1_DbcMatchingTemp_value, in_msgs[i].ECU1_DbcMatchingTemp_value, 0.1);
        ASSERT_EQ(out_msg.ECU1_DbcMatchingRpm_value, in_msgs[i].ECU1_DbcMatchingRpm_value);
    }
}

TEST(PackUnpackTests, test_long_message)
{
    const ECU1_LongMessage_Signals in_msg = {
        .ECU1_Int1_value  = 1,
        .ECU1_Int2_value  = 2,
        .ECU1_Int3_value  = 3,
        .ECU1_Int4_value  = 4,
        .ECU1_Int5_value  = 5,
        .ECU1_Int6_value  = 6,
        .ECU1_Int7_value  = 7,
        .ECU1_Int8_value  = 8,
        .ECU1_Int9_value  = 9,
        .ECU1_Int10_value = 10,
        .ECU1_Int11_value = 11,
        .ECU1_Int12_value = 12,
        .ECU1_Int13_value = 13,
        .ECU1_Int14_value = 14,
        .ECU1_Int15_value = 15,
        .ECU1_Int16_value = 16,
    };

    union
    {
        uint32_t words[16];
        uint8_t  bytes[64];
    } payload = { { 0 } };

    app_canUtils_ECU1_LongMessage_pack(&in_msg, payload.bytes);

    // Confirm encoded payload matches the expected value.
    for (int i = 0; i < 16; i++)
    {
        ASSERT_EQ(payload.words[i], i + 1);
    }

    // Confirm we can decode the payload, which should match the original message.
    ECU1_LongMessage_Signals out_msg;
    app_canUtils_ECU1_LongMessage_unpack(payload.bytes, &out_msg);
    ASSERT_EQ(out_msg.ECU1_Int1_value, 1);
    ASSERT_EQ(out_msg.ECU1_Int2_value, 2);
    ASSERT_EQ(out_msg.ECU1_Int3_value, 3);
    ASSERT_EQ(out_msg.ECU1_Int4_value, 4);
    ASSERT_EQ(out_msg.ECU1_Int5_value, 5);
    ASSERT_EQ(out_msg.ECU1_Int6_value, 6);
    ASSERT_EQ(out_msg.ECU1_Int7_value, 7);
    ASSERT_EQ(out_msg.ECU1_Int8_value, 8);
    ASSERT_EQ(out_msg.ECU1_Int9_value, 9);
    ASSERT_EQ(out_msg.ECU1_Int10_value, 10);
    ASSERT_EQ(out_msg.ECU1_Int11_value, 11);
    ASSERT_EQ(out_msg.ECU1_Int12_value, 12);
    ASSERT_EQ(out_msg.ECU1_Int13_value, 13);
    ASSERT_EQ(out_msg.ECU1_Int14_value, 14);
    ASSERT_EQ(out_msg.ECU1_Int15_value, 15);
    ASSERT_EQ(out_msg.ECU1_Int16_value, 16);
}
