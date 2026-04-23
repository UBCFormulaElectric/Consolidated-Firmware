#include "app_telemRx.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <vector>

#include "app_ntp.hpp"

namespace
{
// Build a frame with the codebase wire format:
//   [0xCC][0x33][body_size][crc:4 LE][body...]
// fake_io_crc returns 0, so all expected_crc fields are 0.
std::vector<uint8_t> buildNtpFrame(uint64_t t1, uint64_t t2)
{
    std::vector<uint8_t> body(17, 0);
    body[0] = 1; // MessageId::Ntp
    std::memcpy(&body[1], &t1, sizeof(uint64_t));
    std::memcpy(&body[9], &t2, sizeof(uint64_t));

    std::vector<uint8_t> frame{ 0xCC, 0x33, static_cast<uint8_t>(body.size()), 0, 0, 0, 0 };
    frame.insert(frame.end(), body.begin(), body.end());
    return frame;
}

void feed(const std::vector<uint8_t> &bytes, uint64_t rx_time_ms = 0)
{
    app::telemRx::ingest(std::span<const uint8_t>{ bytes.data(), bytes.size() }, rx_time_ms);
    app::telemRx::drain();
}
} // namespace

class TelemRxTest : public ::testing::Test
{
  protected:
    void SetUp() override { app::telemRx::reset(); }
};

TEST_F(TelemRxTest, ConsumesValidFrameAndUpdatesTimestamps)
{
    app::ntp::recordT0(100);
    feed(buildNtpFrame(/*t1=*/200, /*t2=*/250), /*rx_time_ms=*/300);

    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t0, 100u);
    EXPECT_EQ(ts.t1, 200u);
    EXPECT_EQ(ts.t2, 250u);
    EXPECT_EQ(ts.t3, 300u);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, JunkBeforeAndBetweenValidFrames)
{
    app::ntp::recordT0(0);

    std::vector<uint8_t> stream{ 0x00, 0xFF, 0xAA, 0xCC, 0x00 }; // junk
    auto                 f1 = buildNtpFrame(11, 12);
    stream.insert(stream.end(), f1.begin(), f1.end());
    stream.push_back(0xDE); // junk
    stream.push_back(0xAD);
    auto f2 = buildNtpFrame(21, 22);
    stream.insert(stream.end(), f2.begin(), f2.end());

    feed(stream, /*rx_time_ms=*/999);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, 21u); // last frame wins
    EXPECT_EQ(ts.t2, 22u);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, FrameSplitAcrossTwoIngests)
{
    app::ntp::recordT0(0);
    auto       frame = buildNtpFrame(7, 8);
    const auto mid   = frame.size() / 2;

    std::vector<uint8_t> first(frame.begin(), frame.begin() + mid);
    std::vector<uint8_t> second(frame.begin() + mid, frame.end());

    feed(first, /*rx_time_ms=*/1);
    EXPECT_GT(app::telemRx::ringSize(), 0u); // partial frame still buffered

    feed(second, /*rx_time_ms=*/2);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, 7u);
    EXPECT_EQ(ts.t2, 8u);
    EXPECT_EQ(ts.t3, 2u); // t3 from the chunk that completed the frame
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, ZeroBodySizeHeaderIsResynced)
{
    app::ntp::recordT0(0);
    std::vector<uint8_t> bad{ 0xCC, 0x33, 0x00, 0, 0, 0, 0 }; // body_size = 0
    auto                 good = buildNtpFrame(33, 44);
    bad.insert(bad.end(), good.begin(), good.end());

    feed(bad, /*rx_time_ms=*/5);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, 33u);
    EXPECT_EQ(ts.t2, 44u);
}
