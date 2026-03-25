#include <gtest/gtest.h>
#include "app_canUtils.hpp"

#define LSHIFT(value, shift) ((uint64_t)value << shift)
#define ENCODE(input, scale, offset) (((input - offset) / scale))
#define TWOS_COMP(input, max) (0xFFFF - input + 1U)

TEST(PackUnpackTests, test_basic_signal_types)
{
    const std::array<app::can_utils::ECU1_BasicSignalTypes_Signals, 3> in_msgs{ {
        // All zeros.
        app::can_utils::ECU1_BasicSignalTypes_Signals{
            false,
            false,
            app::can_utils::EnumExample::ENUM_EX_0,
            0,
            0,
            0,
        },
        // (Mostly) all ones.
        app::can_utils::ECU1_BasicSignalTypes_Signals{
            true,
            true,
            app::can_utils::EnumExample::ENUM_EX_2,
            0xFF,
            0xFFFF,
            0xFFFFFFFF,
        },
        // Some random signal values.
        app::can_utils::ECU1_BasicSignalTypes_Signals{
            true,
            false,
            app::can_utils::EnumExample::ENUM_EX_1,
            123,
            0xFE,
            0xFAFAFAFA,
        },
    } };
    constexpr std::array<uint64_t, 3>                                  expected_payloads{ {
        0,
        0 | LSHIFT(true, 0) | // Boolean1
            LSHIFT(true, 1) | // Boolean2
            LSHIFT(static_cast<uint64_t>(app::can_utils::EnumExample::ENUM_EX_2), 2) | // Enum
            LSHIFT(0xFF, 4) |                                   // Uint8
            LSHIFT(0xFFFF, 12) |                                // Uint16
            LSHIFT(0xFFFFFFFF, 28),                             // Uint32
        0 | LSHIFT(true, 0) |                                   // Boolean1
            LSHIFT(false, 1) |                                  // Boolean2
            LSHIFT(app::can_utils::EnumExample::ENUM_EX_1, 2) | // Enum
            LSHIFT(0xFE, 4) |                                   // Uint8
            LSHIFT(123, 12) |                                   // Uint16
            LSHIFT(0xFAFAFAFA, 28),                             // Uint32
    } };

    for (size_t i = 0; i < 3; i++)
    {
        std::array<uint8_t, 8> payload{};
        in_msgs[i].pack(payload);
        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *reinterpret_cast<uint64_t *>(payload.data()));

        // Confirm we can decode the payload, which should match the original message.
        app::can_utils::ECU1_BasicSignalTypes_Signals out_msg{ payload };
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
    const std::array<app::can_utils::ECU1_DecimalNumbers_Signals, 5> in_msgs{ {
        app::can_utils::ECU1_DecimalNumbers_Signals{ -100.0f, 2.0f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ -54.61f, 2.1243f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 0.0f, 2.5f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 72.34f, 2.784f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 100.0f, 3.0f },
    } };
    constexpr std::array<uint64_t, 5>                                expected_payloads{ {
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
    } };
    const std::array<app::can_utils::ECU1_DecimalNumbers_Signals, 5> expected_unpacked{ {
        app::can_utils::ECU1_DecimalNumbers_Signals{ -100.0f, 2.0f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ -54.7f, 2.12f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 0.0f, 2.5f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 72.3f, 2.78f },
        app::can_utils::ECU1_DecimalNumbers_Signals{ 100.0f, 3.0f },
    } };

    for (size_t i = 0; i < 5; i++)
    {
        std::array<uint8_t, 8> payload{};
        in_msgs[i].pack(payload);

        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *reinterpret_cast<uint64_t *>(payload.data()));

        // Confirm we can decode the payload, which should match the original message.
        const app::can_utils::ECU1_DecimalNumbers_Signals out_msg{ payload };
        ASSERT_NEAR(out_msg.ECU1_Decimal1_value, expected_unpacked[i].ECU1_Decimal1_value, 1e-5);
        ASSERT_NEAR(out_msg.ECU1_Decimal2_value, expected_unpacked[i].ECU1_Decimal2_value, 1e-6);
    }
}

TEST(PackUnpackTests, test_signed_numbers)
{
    const std::array<app::can_utils::ECU1_DbcMatching_Signals, 5> in_msgs{ {
        app::can_utils::ECU1_DbcMatching_Signals{ 0.0f, 0 },
        app::can_utils::ECU1_DbcMatching_Signals{ 1.0f, 1 },
        app::can_utils::ECU1_DbcMatching_Signals{ -1.0f, -1 },
        app::can_utils::ECU1_DbcMatching_Signals{ 3276.7f, 32767 },
        app::can_utils::ECU1_DbcMatching_Signals{ -3276.8f, -32768 },
    } };
    constexpr std::array<uint64_t, 5>                             expected_payloads{ {
        0,
        0 | LSHIFT(ENCODE(1.0, 0.1, 0), 0) |                        // Temp
            LSHIFT(ENCODE(1, 1, 0), 32),                            // RPM
        0 | LSHIFT(TWOS_COMP(ENCODE(1.0, 0.1, 0), 0xFFFF), 0) |     // Temp
            LSHIFT(TWOS_COMP(ENCODE(1, 1, 0), 0xFFFF), 32),         // RPM
        0 | LSHIFT(ENCODE(3276.7, 0.1, 0), 0) |                     // Temp
            LSHIFT(ENCODE(32767, 1, 0), 32),                        // RPM
        0 | LSHIFT(TWOS_COMP(ENCODE(3276.8, 0.1, 0), 0xFFFF), 32) | // Temp
            LSHIFT(TWOS_COMP(ENCODE(32768, 1, 0), 0xFFFF), 0),      // RPM
    } };

    for (size_t i = 0; i < 4; i++)
    {
        std::array<uint8_t, 8> payload{};
        in_msgs[i].pack(payload);

        // Confirm encoded payload matches the expected value.
        ASSERT_EQ(expected_payloads[i], *reinterpret_cast<uint64_t *>(payload.data()));

        // Confirm we can decode the payload, which should match the original message.
        app::can_utils::ECU1_DbcMatching_Signals out_msg{ payload };
        ASSERT_NEAR(out_msg.ECU1_DbcMatchingTemp_value, in_msgs[i].ECU1_DbcMatchingTemp_value, 0.1);
        ASSERT_EQ(out_msg.ECU1_DbcMatchingRpm_value, in_msgs[i].ECU1_DbcMatchingRpm_value);
    }
}

TEST(PackUnpackTests, test_long_message)
{
    const app::can_utils::ECU1_LongMessage_Signals in_msg{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    union
    {
        uint32_t words[16];
        uint8_t  bytes[64];
    } payload = { {} };
    in_msg.pack(payload.bytes);

    // Confirm encoded payload matches the expected value.
    for (int i = 0; i < 16; i++)
    {
        ASSERT_EQ(payload.words[i], i + 1);
    }

    // Confirm we can decode the payload, which should match the original message.
    app::can_utils::ECU1_LongMessage_Signals out_msg{ payload.bytes };
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
