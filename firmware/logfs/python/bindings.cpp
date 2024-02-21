#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <stdlib.h>
#include <iostream>
#include "logfs.h"

namespace py = pybind11;

static LogFsErr _read_wrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
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

static LogFsErr _prog_wrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    // Invoke user-defined program function.
    py::object *context = (py::object *)cfg->context;
    py::bytes   bytes   = py::bytes((char *)buf, cfg->block_size);
    py::object  result  = context->attr("prog")(block, bytes);
    return result.cast<LogFsErr>();
}

static LogFsErr _erase_wrapper(const LogFsCfg *cfg, uint32_t block)
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
        _cfg.read        = _read_wrapper;
        _cfg.prog        = _prog_wrapper;
        _cfg.erase       = _erase_wrapper;

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
    LogFsErr open(LogFsFile &file, char *path) { return logfs_open(&_fs, &file, path); }

    py::tuple read(LogFsFile &file, uint32_t size)
    {
        // Create an empty string to hold the read data.
        std::string buf(size, '\0');
        uint32_t    num_read = logfs_read(&_fs, &file, (void *)buf.data(), size);

        // Return a tuple of (read size, read bytes).
        py::bytes bytes = py::bytes(buf);
        return py::make_tuple(num_read, bytes);
    }

    uint32_t write(LogFsFile &file, const py::bytes bytes, uint32_t size)
    {
        const std::string buf = bytes.cast<std::string>();
        return logfs_write(&_fs, &file, (void *)buf.data(), size);
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
        .value("UNIMPLEMENTED", LOGFS_ERR_UNIMPLEMENTED)
        .value("CORRUPT", LOGFS_ERR_CORRUPT)
        .value("DNE", LOGFS_ERR_DNE)
        .export_values();

    py::class_<LogFsFile>(m, "LogFsFile")
        .def(py::init<>());

    py::class_<PyLogFs>(m, "PyLogFs")
        .def(py::init<uint32_t, uint32_t, py::object&>())
        .def("mount", &PyLogFs::mount)
        .def("format", &PyLogFs::format)
        .def("open", &PyLogFs::open)
        .def("read", &PyLogFs::read)
        .def("write", &PyLogFs::write);
    // clang-format on
}
