#include "app_telemRx.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <vector>

#include "app_crc32.hpp"
#include "app_ntp.hpp"
#include "io_rtc.hpp"

// Test-only spies on the fake RTC, defined in fake_io_rtc.cpp.
namespace fakes::rtc
{
void          setNow(const io::rtc::Time &t);
io::rtc::Time lastSetTime();
bool          wasSetCalled();
void          reset();
} // namespace fakes::rtc

namespace
{
uint32_t computeCrc(const std::vector<uint8_t> &data)
{
    uint32_t c = app::crc32::init();
    c          = app::crc32::update(c, data.data(), data.size());
    return app::crc32::finalize(c);
}

// Build a frame with the codebase wire format:
//   [0xCC][0x33][body_size][crc:4 LE][body...]
std::vector<uint8_t> buildNtpFrame(uint64_t t1, uint64_t t2)
{
    std::vector<uint8_t> body(17, 0);
    body[0] = 1; // MessageId::Ntp
    std::memcpy(&body[1], &t1, sizeof(uint64_t));
    std::memcpy(&body[9], &t2, sizeof(uint64_t));

    uint32_t crc = computeCrc(body);

    std::vector<uint8_t> frame{ 0xCC, 0x33, static_cast<uint8_t>(body.size()) };
    frame.push_back(static_cast<uint8_t>(crc >> 0));
    frame.push_back(static_cast<uint8_t>(crc >> 8));
    frame.push_back(static_cast<uint8_t>(crc >> 16));
    frame.push_back(static_cast<uint8_t>(crc >> 24));
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
    void SetUp() override
    {
        app::telemRx::reset();
        fakes::rtc::reset();
    }
};

TEST_F(TelemRxTest, SlewsRtcToNextSecondBoundary)
{
    // Seed the RTC to 0:0:0.000 (subseconds=999 means "0 ms into this second").
    io::rtc::Time start{};
    start.hours      = 0;
    start.minutes    = 0;
    start.seconds    = 0;
    start.subseconds = 999;
    fakes::rtc::setNow(start);

    // t0 captured "before transmit" = 0 ms.
    app::ntp::recordT0(0);

    // Frame: t1 = 750, t2 = 750. We claim t3 = 0 at receive.
    // theta    = ((750 - 0) + (750 - 0)) / 2 = 750
    // new_ms   = current_rtc_ms (0) + theta (750) = 750
    // round up to next 1000-ms boundary -> 1000 ms = 0:0:1.000
    // msToRtcTime(1000) -> hours=0, minutes=0, seconds=1, subseconds=999
    feed(buildNtpFrame(/*t1=*/750, /*t2=*/750), /*rx_time_ms=*/0);

    EXPECT_TRUE(fakes::rtc::wasSetCalled());
    const auto last = fakes::rtc::lastSetTime();
    EXPECT_EQ(last.hours, 0u);
    EXPECT_EQ(last.minutes, 0u);
    EXPECT_EQ(last.seconds, 1u);
    EXPECT_EQ(last.subseconds, 999u); // PREDIV_S => "0 ms into this second"
}

TEST_F(TelemRxTest, SlewSetsRtcExactlyOnBoundaryWhenAlreadyAligned)
{
    // RTC at 12:00:00.000 (43_200_000 ms-of-day).
    io::rtc::Time start{};
    start.hours      = 12;
    start.minutes    = 0;
    start.seconds    = 0;
    start.subseconds = 999;
    fakes::rtc::setNow(start);

    // Pick t0/t1/t2/t3 so theta is an exact-second multiple.
    // theta = ((t1 - t0) + (t2 - t3)) / 2 = ((1000 - 0) + (1000 - 0)) / 2 = 1000
    app::ntp::recordT0(0);
    feed(buildNtpFrame(/*t1=*/1000, /*t2=*/1000), /*rx_time_ms=*/0);

    // new_ms = 43_200_000 + 1000 = 43_201_000  (already on a second boundary)
    // -> hours=12, minutes=0, seconds=1, subseconds=999
    EXPECT_TRUE(fakes::rtc::wasSetCalled());
    const auto last = fakes::rtc::lastSetTime();
    EXPECT_EQ(last.hours, 12u);
    EXPECT_EQ(last.minutes, 0u);
    EXPECT_EQ(last.seconds, 1u);
    EXPECT_EQ(last.subseconds, 999u);
}

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
