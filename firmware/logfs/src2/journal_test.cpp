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


class JournalTest : public testing::Test
{
  protected:
    static constexpr int kJournalSize = 10;
    static constexpr int kDiskSize    = 100;

    void SetUp()
    {
        auto disk_write = [](const Disk *disk, BlockAddr addr, BlockBuf buf)
        {
            JournalTest *const this_ = reinterpret_cast<JournalTest *>(disk->context);
            memcpy(this_->disk_buf[addr].data(), buf, BLOCK_SIZE);
            return LOGFS_ERR_OK;
        };
        auto disk_read = [](const Disk *disk, BlockAddr addr, BlockBuf buf)
        {
            JournalTest *const this_ = reinterpret_cast<JournalTest *>(disk->context);
            memcpy(buf, this_->disk_buf[addr].data(), BLOCK_SIZE);
            return LOGFS_ERR_OK;
        };

        disk =
            Disk{ .block_count = kDiskSize, .context = this, .rd_only = false, .read = disk_read, .write = disk_write };
        cfg = JournalCfg{ .disk = &disk, .size_blocks = kJournalSize };

        journal_init(&journal, &cfg);
    }

    JournalTag *ReadJournalTag(BlockAddr addr)
    {
        JournalHeader *const header = reinterpret_cast<JournalHeader *>(disk_buf[addr].data());
        return &header->tag;
    }

    Journal                                                journal;
    Disk                                                   disk;
    JournalCfg                                             cfg;
    std::array<std::array<uint8_t, BLOCK_SIZE>, kDiskSize> disk_buf;
};

TEST_F(JournalTest, test_format)
{
    journal_format(&journal);

    for (BlockAddr addr = 0; addr < journal.cfg->size_blocks; addr++)
    {
        const JournalTag *tag = ReadJournalTag(addr);
        ASSERT_EQ(tag->seq_num, addr);
        ASSERT_EQ(tag->addr, BLOCK_ADDR_INVALID);
        ASSERT_EQ(tag->flags, (addr == journal.cfg->size_blocks - 1) ? JOURNAL_FLAG_SYNCED : JOURNAL_FLAG_IGNORE);
    }

    ASSERT_EQ(journal.num_outstanding_commits, 0);
    ASSERT_EQ(journal.head_addr, 0);
    ASSERT_EQ(journal.mounted, false);
}


TEST_F(JournalTest, test_replay)
{
    journal_format(&journal);

    journal_replay(&journal);

    ASSERT_EQ(journal.num_outstanding_commits, 0);

}
