#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <stdlib.h>
#include <iostream>
#include "logfs.h"

namespace py = pybind11;

class PyLogFsCfg
{
  public:
    PyLogFsCfg(
        uint32_t                                     block_size,
        uint32_t                                     block_count,
        std::function<py::tuple(uint32_t)>           read,
        std::function<LogFsErr(uint32_t, py::bytes)> prog,
        std::function<LogFsErr(uint32_t)>            erase)
      : py_read(read), py_prog(prog), py_erase(erase)
    {
        // Init config struct.
        cfg.context     = this;
        cfg.block_size  = block_size;
        cfg.block_count = block_count;
        cfg.read        = &this->read;
        cfg.prog        = &this->prog;
        cfg.erase       = &this->erase;

        // Allocate block cache on heap.
        cfg.block_cache = malloc(block_size);
    }

    ~PyLogFsCfg()
    {
        // Deallocate block cache.
        free(cfg.block_cache);
    }

    LogFsCfg cfg;

  private:
    static LogFsErr read(const LogFsCfg *cfg, uint32_t block, void *buf)
    {
        // Read via user-defined function.
        const PyLogFsCfg *context = (const PyLogFsCfg *)cfg->context;
        py::tuple         result  = context->py_read(block);
        LogFsErr          err     = result[0].cast<LogFsErr>();
        py::bytes         bytes   = result[1].cast<py::bytes>();

        // Copy returned data into buffer.
        const std::string byte_str = bytes.cast<std::string>();
        memcpy(buf, byte_str.data(), cfg->block_size);
        return err;
    }

    static LogFsErr prog(const LogFsCfg *cfg, uint32_t block, void *buf)
    {
        const PyLogFsCfg *context = (const PyLogFsCfg *)cfg->context;
        py::bytes         bytes   = py::bytes((char *)buf, cfg->block_size);
        return context->py_prog(block, bytes);
    }

    static LogFsErr erase(const LogFsCfg *cfg, uint32_t block)
    {
        const PyLogFsCfg *context = (const PyLogFsCfg *)cfg->context;
        return context->py_erase(block);
    }

    std::function<py::tuple(uint32_t)>           py_read;
    std::function<LogFsErr(uint32_t, py::bytes)> py_prog;
    std::function<LogFsErr(uint32_t)>            py_erase;
};

class PyLogFs
{
  public:
    PyLogFs(PyLogFsCfg &cfg) : cfg(cfg){};

    LogFsErr mount(void) { return logfs_mount(&fs, &cfg.cfg); }
    LogFsErr format(void) { return logfs_format(&fs, &cfg.cfg); }
    LogFsErr open(LogFsFile &file, char *path) { return logfs_open(&fs, &file, path); }

    py::tuple read(LogFsFile &file, uint32_t size)
    {
        // Create an empty string to hold the read data.
        std::string buf(size, '\0');
        uint32_t    num_read = logfs_read(&fs, &file, (void *)buf.data(), size);

        // Return a tuple of (read size, read bytes).
        py::bytes bytes = py::bytes(buf);
        return py::make_tuple(num_read, bytes);
    }

    uint32_t write(LogFsFile &file, const py::bytes bytes, uint32_t size)
    {
        const std::string buf = bytes.cast<std::string>();
        return logfs_write(&fs, &file, (void *)buf.data(), size);
    }

  private:
    PyLogFsCfg &cfg;
    LogFs       fs;
};

PYBIND11_MODULE(logfs_lib, m)
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

    py::class_<PyLogFsCfg>(m, "LogFsCfg")
        .def(py::init<uint32_t, uint32_t, std::function<py::tuple(uint32_t)>, std::function<LogFsErr(uint32_t, py::bytes)>, std::function<LogFsErr(uint32_t)>>());

    py::class_<PyLogFs>(m, "LogFs")
        .def(py::init<PyLogFsCfg&>())
        .def("mount", &PyLogFs::mount)
        .def("format", &PyLogFs::format)
        .def("open", &PyLogFs::open)
        .def("read", &PyLogFs::read)
        .def("write", &PyLogFs::write);
    // clang-format on
}
