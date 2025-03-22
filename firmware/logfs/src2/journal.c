#include "journal.h"

#include <string.h>

#include "logfs2.h"
#include "disk.h"
#include "utils.h"

static LogFsErr compareBlockSeqNums(void *context, size_t idx1, size_t idx2, bool *out)
{
    BlockBuf             buf;
    JournalHeader *const header = (JournalHeader *)buf;

    RET_ERR(disk_read(context, (BlockAddr)idx1, buf));
    const uint32_t seq1 = header->tag.seq_num;
    RET_ERR(disk_read(context, (BlockAddr)idx1, buf));
    const uint32_t seq2 = header->tag.seq_num;

    *out = seq1 > seq2;
    return LOGFS_ERR_OK;
}

static bool isBlockCorrupted(void *context, size_t idx)
{
    BlockBuf buf;
    return disk_read(context, idx, buf) != LOGFS_ERR_OK;
}

void journal_init(Journal *journal, const JournalCfg *cfg)
{
    journal->cfg = cfg;
}

bool journal_format(Journal *journal)
{
    BlockBuf buf;
    memset(buf, 0, sizeof(BlockBuf));
    JournalHeader *const header = (JournalHeader *)buf;

    // Fill journal with ignored blocks.
    for (int i = 0; i < journal->cfg->size_blocks - 1; i++)
    {
        header->tag.seq_num = i;
        header->tag.addr    = BLOCK_ADDR_INVALID;
        header->tag.flags   = JOURNAL_FLAG_IGNORE;

        RET_ERR(disk_write(journal->cfg->disk, i, buf));
    }

    // Sync journal.
    header->tag.seq_num = journal->cfg->size_blocks - 1;
    header->tag.addr    = BLOCK_ADDR_INVALID;
    header->tag.flags   = JOURNAL_FLAG_SYNCED;
    RET_ERR(disk_write(journal->cfg->disk, journal->cfg->size_blocks - 1, buf));

    // Set initial values.
    journal->dirty                   = false;
    journal->num_outstanding_commits = 0;
    journal->head_addr               = 0;
    journal->tail_addr               = 0;

    return LOGFS_ERR_OK;
}

LogFsErr journal_replay(Journal *journal)
{
    // Binary search the journal to find the most recently written element (the head).
    uint32_t head_addr;
    RET_ERR(utils_findMaxInSortedCircularBuffer(
        journal->cfg->disk, journal->cfg->size_blocks, compareBlockSeqNums, isBlockCorrupted, &head_addr));

    // From the head, linearly search backwards until you find a "flushed" marker.
    while (true)
    {
        BlockBuf             buf;
        JournalHeader *const header = (JournalHeader *)buf;
        if (disk_read(journal->cfg->disk, head_addr, buf) != LOGFS_ERR_OK)
        {
            continue;
        }

        if (header->tag.flags & JOURNAL_FLAG_SYNCED)
        {
            // Synced, abort!
        }
        else if (!(header->tag.flags & JOURNAL_FLAG_IGNORE))
        {
            // Save state! How to actually represent this state?
        }
    }

    return LOGFS_ERR_OK;
}
