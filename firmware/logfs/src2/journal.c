#include "journal.h"

#include <string.h>

#include "logfs2.h"
#include "disk.h"
#include "utils.h"

void journal_init(Journal *journal, const JournalCfg *cfg)
{
    journal->cfg = cfg;
}

bool journal_format(Journal *journal)
{
    BlockBuf buf;
    memset(buf, 0, sizeof(BlockBuf));

    // Fill journal with ignored blocks.
    for (int i = 0; i < journal->cfg->size_blocks - 1; i++)
    {
        const BlockAddr      addr   = JOURNAL_BASE_ADDR + i;
        JournalHeader *const header = (JournalHeader *)buf;

        header->tag.seq_num = i;
        header->tag.addr    = BLOCK_ADDR_INVALID;
        header->tag.flags   = JOURNAL_FLAG_IGNORE;

        RET_ERR(disk_write(journal->cfg->disk, addr, buf));
    }
}

// bool journal_flush(Journal *journal) {}

// bool journal_replay(Journal *journal) {}

// bool journal_commit(Journal *journal, const JournalTag *tag, BlockBuf buf)
// {
//     // Set tag in block buffer.
//     const JournalHeader *header = (JournalHeader *)buf;
//     memcpy(&header->tag, tag, sizeof(JournalTag));

//     // Write new commit to disk.
//     RET_ERR(disk_write(journal->disk, journal->head_addr, buf));

//     //
//     journal->outstanding_commits[journal->num_outstanding_commits] = tag->addr;
//     journal->num_outstanding_commits += 1;

//     // Increment head address.
//     journal->head_addr += 1;
//     journal->head_addr %= JOURNAL_MAX_NUM_OUTSTANDING_COMMITS;
// }
