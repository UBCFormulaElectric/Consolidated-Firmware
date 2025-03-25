#include "journal.h"

#include <string.h>

#include "logfs2.h"
#include "disk.h"
#include "utils.h"

#define DEC_ADDR(journal, addr)               \
    if (addr == 0)                            \
    {                                         \
        addr = journal->cfg->size_blocks - 1; \
    }                                         \
    else                                      \
    {                                         \
        addr--;                               \
    }

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

static void commitListAdd(Journal *journal, BlockAddr target_addr, BlockAddr commit_addr)
{
    bool dirty = false;
    for (int i = 0; i < journal->num_outstanding_commits; i++)
    {
        if (journal->commit_list[i].target_addr == target_addr)
        {
            dirty = true;
        }
    }

    if (!dirty)
    {
        journal->commit_list[journal->num_outstanding_commits].target_addr = target_addr;
        journal->commit_list[journal->num_outstanding_commits].commit_addr = commit_addr;
        journal->num_outstanding_commits++;
    }
}

void journal_init(Journal *journal, const JournalCfg *cfg)
{
    journal->cfg                     = cfg;
    journal->num_outstanding_commits = 0;
    journal->head_addr               = 0;
    journal->mounted                 = false;
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
    journal->num_outstanding_commits = 0;
    journal->head_addr               = 0;
    journal->mounted                 = false;

    return LOGFS_ERR_OK;
}

LogFsErr journal_replay(Journal *journal)
{
    // Binary search the journal to find the most recently written element (the head).
    uint32_t commit_addr;
    RET_ERR(utils_findMaxInSortedCircularBuffer(
        journal->cfg->disk, journal->cfg->size_blocks, compareBlockSeqNums, isBlockCorrupted, &commit_addr));

    BlockBuf             buf;
    JournalHeader *const header = (JournalHeader *)buf;

    // From the head, linearly search backwards until you find a "flushed" marker.
    for (;;)
    {
        if (disk_read(journal->cfg->disk, commit_addr, buf) != LOGFS_ERR_OK)
        {
            // Commit is invalid, skip over it.
            DEC_ADDR(journal, commit_addr);
            continue;
        }

        if (header->tag.flags & JOURNAL_FLAG_SYNCED)
        {
            // All previous commits were synced.
            break;
        }

        if (header->tag.flags & JOURNAL_FLAG_IGNORE)
        {
            // Commit should be ignored.
            DEC_ADDR(journal, commit_addr);
            continue;
        }

        commitListAdd(journal, header->tag.addr, commit_addr);
        DEC_ADDR(journal, commit_addr);
    }

    return LOGFS_ERR_OK;
}

LogFsErr journal_commit(Journal *journal, BlockBuf buf)
{
    JournalHeader *const header = (JournalHeader *)buf;
    header->tag.seq_num         = journal->seq_num + 1;
    // todo...
    journal->seq_num++;
}
