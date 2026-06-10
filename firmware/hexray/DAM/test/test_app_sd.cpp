#include "app_sd.hpp"

#include "io_fileSystems.hpp"
#include "io_rtc.hpp"
#include "io_timeFake.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

// Test-only spies on the fake RTC, defined in fake_io_rtc.cpp.
namespace fakes::rtc
{
void setEpochMs(uint64_t epoch_ms);
void reset();
} // namespace fakes::rtc

namespace
{
// 2026-01-01 00:00:00.000 UTC — any epoch in the RTC's 2000..2099 range works.
constexpr uint64_t kEpoch2026 = 1'767'225'600'000ULL;

// The host-filesystem fake (damMocks.cpp) maps "/x" to "./x", so app_sd's
// artifacts land in the test working directory.
constexpr const char *kBootcountPath = "./bootcount.txt";

bool isBootLogName(const std::string &name)
{
    return name.starts_with("boot_") && name.ends_with(".bin");
}

// Find the "./boot_<num>_<hash>.bin" log file created by init_fs().
std::filesystem::path findBootLogPath()
{
    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        if (entry.is_regular_file() && isBootLogName(entry.path().filename().string()))
            return entry.path();
    }
    return {};
}

void removeArtifacts()
{
    std::error_code ec;
    std::filesystem::remove(kBootcountPath, ec);
    for (auto path = findBootLogPath(); !path.empty(); path = findBootLogPath())
        std::filesystem::remove(path, ec);
}

class AppSdTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Start at t=1ms (project convention from EcuTestBase) and seed the RTC,
        // so the boot-filename hash and metadata basetime are well defined.
        fakes::time::setTime(1);
        fakes::rtc::reset();
        fakes::rtc::setEpochMs(kEpoch2026);

        // app::sd keeps static state: the log stays open for the whole test
        // binary, so wipe stale artifacts and run init_fs() exactly once.
        static const bool init_ok = []
        {
            removeArtifacts();
            return app::sd::init_fs().has_value();
        }();
        ASSERT_TRUE(init_ok);
    }
};
} // namespace

// --- init ------------------------------------------------------------------

TEST_F(AppSdTest, InitOpensLogFile)
{
    EXPECT_TRUE(app::sd::isLogOpen());
    EXPECT_LT(app::sd::getLogFd(), io::FileSystem::MAX_FILE_NUMBER);
}

TEST_F(AppSdTest, InitCreatesBootcountAndBootLogOnDisk)
{
    EXPECT_TRUE(std::filesystem::exists(kBootcountPath));
    EXPECT_FALSE(findBootLogPath().empty());
}

TEST_F(AppSdTest, InitIsIdempotent)
{
    const uint32_t fd_before = app::sd::getLogFd();

    // A second init must succeed without reopening anything.
    ASSERT_TRUE(app::sd::init_fs().has_value());
    EXPECT_TRUE(app::sd::isLogOpen());
    EXPECT_EQ(app::sd::getLogFd(), fd_before);
}

// --- log file descriptor ---------------------------------------------------

TEST_F(AppSdTest, LogFdIsWritableAndSyncable)
{
    // The fd handed out by getLogFd() must be valid in the filesystem: both
    // write and sync should succeed on it.
    std::array<uint8_t, 4> payload{ 0xDE, 0xAD, 0xBE, 0xEF };
    EXPECT_TRUE(fs.write(app::sd::getLogFd(), payload, payload.size()).has_value());
    EXPECT_TRUE(fs.sync(app::sd::getLogFd()).has_value());
}

// --- deferred service requests ----------------------------------------------

TEST_F(AppSdTest, ServiceWithNoPendingRequestsIsANoOp)
{
    const auto size_before = std::filesystem::file_size(findBootLogPath());
    app::sd::service();
    EXPECT_EQ(std::filesystem::file_size(findBootLogPath()), size_before);
    EXPECT_TRUE(app::sd::isLogOpen());
}

TEST_F(AppSdTest, RequestMetadataUpdateIsDeferredUntilService)
{
    const auto log_path    = findBootLogPath();
    const auto size_before = std::filesystem::file_size(log_path);

    // The request alone must not touch the SD (it may come from another task).
    app::sd::requestMetadataUpdate();
    EXPECT_EQ(std::filesystem::file_size(log_path), size_before);

    // service() (run on TaskLogging) performs the actual metadata write.
    app::sd::service();
    EXPECT_GT(std::filesystem::file_size(log_path), size_before);
}

TEST_F(AppSdTest, MetadataRequestIsConsumedByService)
{
    app::sd::requestMetadataUpdate();
    app::sd::service();

    // The flag was consumed: a second service() must not write again.
    const auto size_after_first = std::filesystem::file_size(findBootLogPath());
    app::sd::service();
    EXPECT_EQ(std::filesystem::file_size(findBootLogPath()), size_after_first);
}

TEST_F(AppSdTest, RequestSyncIsServicedWithoutClosingLog)
{
    app::sd::requestSync();
    app::sd::service();
    EXPECT_TRUE(app::sd::isLogOpen());
}
