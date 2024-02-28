#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <stdlib.h>
#include <iostream>
#include "logfs.h"

namespace py = pybind11;

static LogFsErr _readWrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    // Invoke user-defined read function.
    py::object *context = (py::object *)cfg->context;
    py::tuple   result  = context->attr("read")(block);
    LogFsErr    err     = result[0].cast<LogFsErr>();
    py::bytes   bytes   = result[1].cast<py::bytes>();

    // Copy returned data into buffer.
    const std::string byte_str = bytes.cast<std::string>();
    memcpy(buf, byte_str.data(), cfg->block_size);
    return err;
}

static LogFsErr _progWrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    // Invoke user-defined program function.
    py::object *context = (py::object *)cfg->context;
    py::bytes   bytes   = py::bytes((char *)buf, cfg->block_size);
    py::object  result  = context->attr("prog")(block, bytes);
    return result.cast<LogFsErr>();
}

static LogFsErr _eraseWrapper(const LogFsCfg *cfg, uint32_t block)
{
    // Invoke user-defined erase function.
    py::object *context = (py::object *)cfg->context;
    py::object  result  = context->attr("erase")(block);
    return result.cast<LogFsErr>();
}

class PyLogFs
{
  public:
    PyLogFs(uint32_t block_size, uint32_t block_count, py::object context) : _context(context)
    {
        // Init config struct.
        _cfg.block_size  = block_size;
        _cfg.block_count = block_count;
        _cfg.context     = &_context;
        _cfg.read        = _readWrapper;
        _cfg.prog        = _progWrapper;
        _cfg.erase       = _eraseWrapper;

        // Allocate block cache on heap.
        _cfg.block_cache = malloc(block_size);
    };

    ~PyLogFs()
    {
        // Deallocate block cache.
        free(_cfg.block_cache);
    }

    LogFsErr mount(void) { return logfs_mount(&_fs, &_cfg); }

    LogFsErr format(void) { return logfs_format(&_fs, &_cfg); }

    py::tuple bootCount(void)
    {
        uint32_t       count;
        const LogFsErr err = logfs_bootCount(&_fs, &count);

        // Return a tuple of (error, boot count).
        return py::make_tuple(err, count);
    }

    LogFsErr open(LogFsFile &file, char *path) { return logfs_open(&_fs, &file, path); }

    py::tuple read(LogFsFile &file, uint32_t size, LogFsRead mode)
    {
        // Create an empty string to hold the read data.
        std::string    buf(size, '\0');
        uint32_t       num_read;
        const LogFsErr err = logfs_read(&_fs, &file, (void *)buf.data(), size, mode, &num_read);

        // Return a tuple of (error, read size, read bytes).
        py::bytes bytes = py::bytes(buf);
        return py::make_tuple(err, num_read, bytes);
    }

    py::tuple write(LogFsFile &file, const py::bytes bytes, uint32_t size)
    {
        // Write to disk.
        const std::string buf = bytes.cast<std::string>();
        uint32_t          num_written;
        const LogFsErr    err = logfs_write(&_fs, &file, (void *)buf.data(), size, &num_written);

        // Return a tuple of (error, write size).
        return py::make_tuple(err, num_written);
    }

    py::tuple firstPath(void)
    {
        // Get the first file's path.
        LogFsPath         path;
        const LogFsErr    err = logfs_firstPath(&_fs, &path);
        const std::string path_str(path.path);

        // Return a tuple of (error, path, path string).
        return py::make_tuple(err, path, path_str);
    }

    py::tuple nextPath(LogFsPath path)
    {
        // Get the next file's path.
        const LogFsErr    err = logfs_nextPath(&_fs, &path);
        const std::string path_str(path.path);

        // Return a tuple of (error, path, path string).
        return py::make_tuple(err, path, path_str);
    }

  private:
    LogFsCfg   _cfg;
    LogFs      _fs;
    py::object _context;
};

PYBIND11_MODULE(logfs_src, m)
{
    // clang-format off
    py::enum_<LogFsErr>(m, "LogFsErr")
        .value("OK", LOGFS_ERR_OK)
        .value("IO", LOGFS_ERR_IO)
        .value("CORRUPT", LOGFS_ERR_CORRUPT)
        .value("INVALID_ARG", LOGFS_ERR_INVALID_ARG)
        .value("INVALID_PATH", LOGFS_ERR_INVALID_PATH)
        .value("INVALID_BLOCK", LOGFS_ERR_INVALID_BLOCK)
        .value("UNMOUNTED", LOGFS_ERR_UNMOUNTED)
        .value("NOMEM", LOGFS_ERR_NOMEM)
        .value("EMPTY", LOGFS_ERR_EMPTY)
        .value("UNIMPLEMENTED", LOGFS_ERR_UNIMPLEMENTED)
        .export_values();

    py::enum_<LogFsRead>(m, "PyLogFsRead")
        .value("START", LOGFS_READ_START)
        .value("ITER", LOGFS_READ_ITER)
        .export_values();

    py::class_<LogFsFile>(m, "LogFsFile")
        .def(py::init<>());

    py::class_<LogFsPath>(m, "PyLogFsPath")
        .def(py::init<>());

    py::class_<PyLogFs>(m, "PyLogFs")
        .def(py::init<uint32_t, uint32_t, py::object&>())
        .def("mount", &PyLogFs::mount)
        .def("format", &PyLogFs::format)
        .def("boot_count", &PyLogFs::bootCount)
        .def("open", &PyLogFs::open)
        .def("read", &PyLogFs::read)
        .def("write", &PyLogFs::write)
        .def("first_path", &PyLogFs::firstPath)
        .def("next_path", &PyLogFs::nextPath);
    // clang-format on
}
