#include <array>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>
#include <memory>
#include <string.h>
#include <vector>

#include "disk.h"
#include "journal.h"
#include "logfs2.h"

#include <iostream>

class JournalTest : public testing::Test
{
  protected:
    static constexpr int kJournalSize = 10;
    static constexpr int kDiskSize    = 100;

    void SetUp()
    {
        auto disk_write = [](const Disk *disk, BlockAddr addr, BlockBuf buf)
        {
            JournalTest *const this_ = static_cast<JournalTest *const>(disk->context);
            memcpy(this_->disk_buf[addr].data(), buf, BLOCK_SIZE);
            return LOGFS_ERR_OK;
        };
        auto disk_read = [](const Disk *disk, BlockAddr addr, BlockBuf buf)
        {
            JournalTest *const this_ = static_cast<JournalTest *const>(disk->context);
            memcpy(buf, this_->disk_buf[addr].data(), BLOCK_SIZE);
            return LOGFS_ERR_OK;
        };

        disk =
            Disk{ .block_count = kDiskSize, .context = this, .rd_only = false, .read = disk_read, .write = disk_write };
        cfg = JournalCfg{ .disk = &disk, .size_blocks = kJournalSize };

        journal_init(&journal, &cfg);
    }

    Journal                                                journal;
    Disk                                                   disk;
    JournalCfg                                             cfg;
    std::array<std::array<uint8_t, BLOCK_SIZE>, kDiskSize> disk_buf;
};

TEST_F(JournalTest, test_format)
{
    journal_format(&journal);
}
