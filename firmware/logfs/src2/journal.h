#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "logfs2.h"
#include "disk.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define JOURNAL_BASE_ADDR ((BlockAddr)0)
#define JOURNAL_MAX_NUM_OUTSTANDING_COMMITS 10

#define JOURNAL_FLAG_IGNORE (1 << 0)
#define JOURNAL_FLAG_CHECKPOINT (1 << 1)

    typedef struct
    {
        uint32_t  seq_num;
        BlockAddr addr;
        uint32_t  flags;
    } JournalTag;

    typedef struct
    {
        uint32_t   crc;
        JournalTag tag;
    } JournalHeader;

    typedef struct  {
        Disk* disk;
        size_t    size_blocks;
    } JournalCfg;

    typedef struct
    {
        const JournalCfg* cfg;
        bool      dirty;
        size_t    num_outstanding_commits;
        BlockAddr outstanding_commits[JOURNAL_MAX_NUM_OUTSTANDING_COMMITS];
        BlockAddr head_addr;
        BlockAddr tail_addr;
    } Journal;

    void journal_init(Journal* journal, const JournalCfg* cfg);
    bool journal_format(Journal *journal);
    // bool journal_flush(Journal *journal);
    // bool journal_replay(Journal *journal);

    // bool journal_commit(Journal *journal, const JournalTag *tag, BlockBuf buf);

#ifdef __cplusplus
}
#endif
