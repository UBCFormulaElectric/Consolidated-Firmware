from libc.stdint cimport uint8_t, int32_t, uint32_t

cdef extern from "logfs.h":

    ctypedef enum LogFsErr:
        LOGFS_ERR_OK = 0
        LOGFS_ERR_UNIMPLEMENTED = 1
        LOGFS_ERR_CORRUPT = 2
        LOGFS_ERR_DNE = 3

    ctypedef struct LogFsCfg:
        uint32_t block_size
        uint32_t block_count
        LogFsErr (*read)(uint32_t block, void *buf)
        LogFsErr (*prog)(uint32_t block, void *buf)
        LogFsErr (*erase)(uint32_t block);
        uint8_t *block_buf;

    ctypedef struct LogFs:
        const LogFsCfg *cfg;
        uint32_t cur_file;
        uint32_t head;

    int logfs_add(int a, int b)
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);

cdef int _read(uint32_t block, void* buf) noexcept:
    ctx = <object>c.context
    data = ctx.user_context.read(ctx, block, off, size)
    memcpy(buffer, <char *>data, size)
    return 0


cdef int _lfs_prog(const lfs_config *c, lfs_block_t block, lfs_off_t off, const void * buffer, lfs_size_t size) noexcept:
    ctx = <object>c.context
    data = (<char*>buffer)[:size]
    return ctx.user_context.prog(ctx, block, off, data)


cdef int _lfs_erase(const lfs_config *c, lfs_block_t block) noexcept:
    ctx = <object>c.context
    return ctx.user_context.erase(ctx, block)


cdef class LogFsConfig:
    cdef LogFsCfg _impl

    def __cinit__(self):
        self._impl.read = &_lfs_read
        self._impl.prog = &_lfs_prog
        self._impl.erase = &_lfs_erase
        self._impl.sync = &_lfs_sync

    def __init__(
        self,
        context=None,
        block_size: int = 512,
        block_count: int = 64,
    ):
        self._impl.block_size = block_size
        self._impl.block_count = block_count

def add(a: int, b: int) -> int:
    return logfs_add(a, b)

