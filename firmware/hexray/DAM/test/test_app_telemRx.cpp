#include "app_telemRx.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <vector>

#include "app_crc32.hpp"
#include "app_ntp.hpp"
#include "app_epochClock.hpp"
#include "io_rtc.hpp"

// Test-only spies on the fake RTC, defined in fake_io_rtc.cpp.
namespace fakes::rtc
{
void          setNow(const io::rtc::Time &t);
void          setEpochMs(uint64_t epoch_ms);
io::rtc::Time lastSetTime();
bool          wasSetCalled();
void          reset();
} // namespace fakes::rtc

namespace
{
// Whole-midnight Unix epoch anchor used to keep time-of-day assertions
// simple (time-of-day(kAnchor) = 00:00:00.000). 2000-01-01 00:00:00 UTC.
constexpr uint64_t kAnchor = 946'684'800'000ULL;

// Build a frame with the codebase wire format:
//   [0xCC][0x33][body_size][crc:4 LE][body...]
std::array<uint8_t, 24> buildNtpFrame(uint64_t t1, uint64_t t2)
{
    std::array<uint8_t, 17> body{};
    body[0] = 1; // MessageId::Ntp
    std::memcpy(&body[1], &t1, sizeof(uint64_t));
    std::memcpy(&body[9], &t2, sizeof(uint64_t));

    const uint32_t crc = app::crc32::finalize(app::crc32::update(app::crc32::init(), body.data(), body.size()));

    std::array<uint8_t, 24> frame{
        0xCC,
        0x33,
        static_cast<uint8_t>(body.size()),
        static_cast<uint8_t>(crc >> 0),
        static_cast<uint8_t>(crc >> 8),
        static_cast<uint8_t>(crc >> 16),
        static_cast<uint8_t>(crc >> 24),
    };
    std::memcpy(&frame[7], body.data(), body.size());
    return frame;
}

void feed(std::span<const uint8_t> bytes)
{
    app::telemRx::ingest(bytes);
    app::telemRx::drain();
}

// t3 is captured by the parser from the RTC at frame-completion time,
// so tests that care about a specific t3 seed the fake RTC accordingly.
void setRtcEpoch(uint64_t epoch_ms)
{
    fakes::rtc::setEpochMs(epoch_ms);
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
    // Seed RTC to kAnchor (midnight, time-of-day = 0). t0 = kAnchor.
    fakes::rtc::setEpochMs(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());

    // Frame: t1 = kAnchor + 750, t2 = kAnchor + 750. RTC reads t3 = kAnchor.
    // theta    = ((t1 - t0) + (t2 - t3)) / 2 = 750
    // new_ms   = kAnchor + 750
    // round up to next 1000-ms boundary -> kAnchor + 1000 -> 00:00:01.000
    feed(buildNtpFrame(kAnchor + 750, kAnchor + 750));

    EXPECT_TRUE(fakes::rtc::wasSetCalled());
    const auto last = fakes::rtc::lastSetTime();
    EXPECT_EQ(last.hours, 0u);
    EXPECT_EQ(last.minutes, 0u);
    EXPECT_EQ(last.seconds, 1u);
    EXPECT_EQ(last.subseconds, 999u); // PREDIV_S => "0 ms into this second"
}

TEST_F(TelemRxTest, SlewSetsRtcExactlyOnBoundaryWhenAlreadyAligned)
{
    // RTC at 12:00:00.000 = kAnchor + 12 h.
    constexpr uint64_t noon = kAnchor + 12ULL * 3600ULL * 1000ULL;
    fakes::rtc::setEpochMs(noon);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    // theta = ((noon+1000 - noon) + (noon+1000 - noon)) / 2 = 1000
    // new_ms = noon + 1000 (already on a second boundary) -> 12:00:01.000
    feed(buildNtpFrame(noon + 1000, noon + 1000));

    EXPECT_TRUE(fakes::rtc::wasSetCalled());
    const auto last = fakes::rtc::lastSetTime();
    EXPECT_EQ(last.hours, 12u);
    EXPECT_EQ(last.minutes, 0u);
    EXPECT_EQ(last.seconds, 1u);
    EXPECT_EQ(last.subseconds, 999u);
}

TEST_F(TelemRxTest, ConsumesValidFrameAndUpdatesTimestamps)
{
    setRtcEpoch(kAnchor + 100); // RTC at capture-T0 time
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    setRtcEpoch(kAnchor + 300); // parser will capture t3 = kAnchor + 300
    feed(buildNtpFrame(/*t1=*/kAnchor + 200, /*t2=*/kAnchor + 250));

    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t0, kAnchor + 100);
    EXPECT_EQ(ts.t1, kAnchor + 200);
    EXPECT_EQ(ts.t2, kAnchor + 250);
    EXPECT_EQ(ts.t3, kAnchor + 300);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, JunkBeforeAndBetweenValidFrames)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());

    std::vector<uint8_t> stream{ 0x00, 0xFF, 0xAA, 0xCC, 0x00 }; // junk
    auto                 f1 = buildNtpFrame(kAnchor + 11, kAnchor + 12);
    stream.insert(stream.end(), f1.begin(), f1.end());
    stream.push_back(0xDE); // junk
    stream.push_back(0xAD);
    auto f2 = buildNtpFrame(kAnchor + 21, kAnchor + 22);
    stream.insert(stream.end(), f2.begin(), f2.end());

    feed(stream);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, kAnchor + 21); // last frame wins
    EXPECT_EQ(ts.t2, kAnchor + 22);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, FrameSplitAcrossTwoIngests)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    auto                     frame = buildNtpFrame(kAnchor + 7, kAnchor + 8);
    const auto               mid   = frame.size() / 2;
    std::span<const uint8_t> bytes{ frame };

    setRtcEpoch(kAnchor + 1);
    feed(bytes.first(mid));
    EXPECT_GT(app::telemRx::ringSize(), 0u); // partial frame still buffered

    setRtcEpoch(kAnchor + 2); // RTC at the moment the frame completes
    feed(bytes.subspan(mid));
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, kAnchor + 7);
    EXPECT_EQ(ts.t2, kAnchor + 8);
    EXPECT_EQ(ts.t3, kAnchor + 2); // t3 captured by the parser when the frame finished
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, ZeroBodySizeHeaderIsResynced)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    std::vector<uint8_t> bad{ 0xCC, 0x33, 0x00, 0, 0, 0, 0 }; // body_size = 0
    auto                 good = buildNtpFrame(kAnchor + 33, kAnchor + 44);
    bad.insert(bad.end(), good.begin(), good.end());

    feed(bad);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, kAnchor + 33);
    EXPECT_EQ(ts.t2, kAnchor + 44);
}
