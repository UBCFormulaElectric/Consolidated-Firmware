#include "journal.h"

#include <string.h>

#include "logfs2.h"
#include "disk.h"
#include "utils.h"

static int circularSearch(int arr[], int size)
{
    // Edge cases
    if (size == 0)
        return -1; // Empty array
    if (size == 1)
        return arr[0]; // Single element

    // Check if the array is not rotated
    if (arr[0] < arr[size - 1])
    {
        return arr[size - 1]; // Last element is the maximum
    }

    int left  = 0;
    int right = size - 1;

    while (left <= right)
    {
        // If there are only 1 or 2 elements left
        if (left == right)
        {
            return arr[left];
        }
        if (left + 1 == right)
        {
            return arr[left] > arr[right] ? arr[left] : arr[right];
        }

        int mid = left + (right - left) / 2;

        // If mid is the maximum
        if (arr[mid] > arr[mid + 1] && arr[mid] > arr[mid - 1])
        {
            return arr[mid];
        }

        // If we're in the ascending part of the array
        if (arr[left] <= arr[mid])
        {
            // Check if the maximum is in this part
            if (arr[mid] > arr[right])
            {
                // Maximum is in the right half of this ascending part
                left = mid + 1;
            }
            else
            {
                // Maximum is in the left half
                right = mid - 1;
            }
        }
        // If we're in the descending-then-ascending part
        else
        {
            // Maximum is in the left half
            right = mid - 1;
        }
    }

    return -1; // Should never reach here if input is valid
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
    header->tag.flags   = JOURNAL_FLAG_CHECKPOINT;
    RET_ERR(disk_write(journal->cfg->disk, journal->cfg->size_blocks - 1, buf));

    // Set initial values.
    journal->dirty                   = false;
    journal->num_outstanding_commits = 0;
    journal->head_addr               = 0;
    journal->tail_addr               = 0;

    return LOGFS_ERR_OK;
}

// bool journal_flush(Journal *journal) {}

bool journal_replay(Journal *journal) {}

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
