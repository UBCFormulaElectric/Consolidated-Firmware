#pragma once

#include "logfs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    Power loss resilience for critical data is guaranteed with "pairs." Pairs consist of 2 blocks: Block 0 and Block 1.
    Writes to pairs are done in an alternating order, so for ex. block 0, then block 1, then block 0, etc. This means
    that if a write fails due to power loss, we can roll back to the other pair block.

    How do we know which element in the pair is the most recent? If either element is invalid, we just have to
    pick the valid one. If both are valid, we use the "sequence number" stored in each block.

    Sequence numbers have 4 possible values, and are initialized to zero. Sequence number comparsions are made like:
    - 4 > 3
    - 3 > 2
    - 2 > 1
    - 0 > 4

    Each element of the pair has its own sequence number that is written to disk. Upon reading a pair with an unknown
    state, the two sequence numbers can be compared as above, and the element with the most significant number will be
    taken to be the most recent. So, when alternating between element writes, we need to always increment the sequence
    number by 1. This way, our most recent write will always have the greatest sequence number, which we want!

    A few optimizations:
    1. We can say that even sequence numbers correspond to Block 0 being the most recent, and odd correspond to Block 1
    being the most recent. Then we only need to keep track of the sequence number.
    2. We can cache sequence number for blocks we're going to want to use again. Then, we don't need to figure out which
    is the most recent for later reads/writes. Be careful! If cacheing is not done properly, you could easily use
    invalid/old data.
    */

    LogFsErr disk_write(const LogFs *fs, uint32_t block, void *buf);
    LogFsErr disk_read(const LogFs *fs, uint32_t block, void *buf);

    LogFsErr disk_changeCache(const LogFs *fs, LogFsCache *cache, uint32_t block, bool write_back, bool fetch);
    LogFsErr disk_syncCache(const LogFs *fs, const LogFsCache *cache);

    void     disk_newPair(LogFsPair *pair, uint32_t block);
    LogFsErr disk_fetchPair(const LogFs *fs, LogFsPair *pair, uint32_t block);
    LogFsErr disk_writePair(const LogFs *fs, LogFsPair *pair);
    LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair);

#ifdef __cplusplus
}
#endif

// change cache, write back
// chance cache, fetch
// change cache, write back, fetch