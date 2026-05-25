#include "app_telemRx.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <variant>
#include <vector>

#include "app_crc32.hpp"
#include "app_ntp.hpp"
#include "app_epochClock.hpp"
#include "io_rtc.hpp"
#include "io_telemMessage.hpp"
#include "io_telemQueue.hpp"

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

// Mirrors the private ringCapacity in app_telemRx.cpp; kept in sync by hand
// since it isn't exported. 22 * 24-byte frames = 528.
constexpr std::size_t kRingCapacity = 528;

// Build a frame with the codebase wire format:
//   [0xCC][0x33][body_size][crc:4 LE][body...]
std::array<uint8_t, 24> buildNtpFrame(uint64_t t1, uint64_t t2)
{
    std::array<uint8_t, 17> body{};
    body[0] = 1; // MessageId::Ntp
    std::memcpy(&body[1], &t1, sizeof(uint64_t));
    std::memcpy(&body[9], &t2, sizeof(uint64_t));

    const uint32_t crc = app::crc32::finalize(app::crc32::update(app::crc32::init(), body.data(), body.size()));

    std::array<uint8_t, 24> frame{ {
        0xCC,
        0x33,
        static_cast<uint8_t>(body.size()),
        static_cast<uint8_t>(crc >> 0),
        static_cast<uint8_t>(crc >> 8),
        static_cast<uint8_t>(crc >> 16),
        static_cast<uint8_t>(crc >> 24),
    } };
    std::memcpy(&frame[7], body.data(), body.size());
    return frame;
}

// Build a minimal valid frame with a custom 1-byte body. Used to exercise
// dispatch branches (unknown ids, Remote_NTP) without depending on the NTP
// body shape.
std::array<uint8_t, 8> buildSingleByteFrame(uint8_t id)
{
    const std::array<uint8_t, 1> body{ { id } };
    const uint32_t crc = app::crc32::finalize(app::crc32::update(app::crc32::init(), body.data(), body.size()));

    return std::array<uint8_t, 8>{ {
        0xCC,
        0x33,
        static_cast<uint8_t>(body.size()),
        static_cast<uint8_t>(crc >> 0),
        static_cast<uint8_t>(crc >> 8),
        static_cast<uint8_t>(crc >> 16),
        static_cast<uint8_t>(crc >> 24),
        id,
    } };
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
    EXPECT_EQ(last.subseconds, 1023u); // subseconds == PREDIV_S means "0 ms into this second"
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
    EXPECT_EQ(last.subseconds, 1023u);
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

TEST_F(TelemRxTest, CrcMismatchResyncsAndRecovers)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());

    auto bad = buildNtpFrame(kAnchor + 1, kAnchor + 2);
    bad[10] ^= 0xFF; // corrupt one body byte; CRC will fail
    auto good = buildNtpFrame(kAnchor + 55, kAnchor + 66);

    std::vector<uint8_t> stream(bad.begin(), bad.end());
    stream.insert(stream.end(), good.begin(), good.end());

    feed(stream);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, kAnchor + 55);
    EXPECT_EQ(ts.t2, kAnchor + 66);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, OversizedBodyHeaderIsResynced)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    // body_size = 0xFF exceeds maxBodySize (100). Parser must slide one byte
    // and keep hunting, eventually consuming the good frame.
    std::vector<uint8_t> bad{ 0xCC, 0x33, 0xFF, 0, 0, 0, 0 };
    auto                 good = buildNtpFrame(kAnchor + 77, kAnchor + 88);
    bad.insert(bad.end(), good.begin(), good.end());

    feed(bad);
    const auto &ts = app::ntp::timestamps();
    EXPECT_EQ(ts.t1, kAnchor + 77);
    EXPECT_EQ(ts.t2, kAnchor + 88);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, UnknownMessageIdIsDropped)
{
    setRtcEpoch(kAnchor);
    auto frame = buildSingleByteFrame(0x7F); // not a real MessageId

    feed(frame);
    EXPECT_FALSE(fakes::rtc::wasSetCalled());
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, RemoteNtpEnqueuesTxMessage)
{
    // None of the tests above push to telem_tx_queue, so it is empty here.
    setRtcEpoch(kAnchor);
    auto frame = buildSingleByteFrame(0x02); // MessageId::Remote_NTP

    feed(frame);
    auto entry = telem_tx_queue.pop();
    ASSERT_TRUE(entry.has_value());
    EXPECT_TRUE(std::holds_alternative<io::telemMessage::NTPMsg>(*entry));
    EXPECT_FALSE(fakes::rtc::wasSetCalled()); // remote-NTP doesn't touch the RTC
}

TEST_F(TelemRxTest, IngestOverflowDropsIncomingBytes)
{
    // Single push larger than the ring; RingBuffer::push rejects the whole
    // span, so ringSize stays at 0.
    std::vector<uint8_t> junk(kRingCapacity + 1, 0x00);
    app::telemRx::ingest(junk);
    EXPECT_EQ(app::telemRx::ringSize(), 0u);
}

TEST_F(TelemRxTest, TryBeginAndCaptureT0ReentryReturnsFalse)
{
    setRtcEpoch(kAnchor);
    ASSERT_TRUE(app::ntp::tryBeginAndCaptureT0());
    // No frame consumed yet, so the in-progress flag is still set.
    EXPECT_FALSE(app::ntp::tryBeginAndCaptureT0());
    // Finish the exchange so the module's in-progress flag is clear for
    // subsequent tests (no per-test reset hook exists for app::ntp).
    feed(buildNtpFrame(kAnchor, kAnchor));
}
