#include "logfs.h"
#include "crc.h"

static LogFsErr inline logfs_writeCache(const LogFs *fs, uint32_t block)
{
    crc_stampBlock(fs);
    RET_ERR(fs->cfg->write(fs->cfg, block, fs->cfg->cache));
}

static void inline logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;
    fs->mounted                             = false;
    fs->out_of_memory                       = false;
    fs->max_path_len                        = MIN(fs->cfg->block_size - sizeof(LogFsBlock_File), LOGFS_PATH_BYTES);
}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    // Write first block and erase next.
    RET_ERR(fs->cfg->erase(fs->cfg, LOGFS_ORIGIN + LOGFS_NUM_COPIES + 1));
    return LOGFS_ERR_OK;
}