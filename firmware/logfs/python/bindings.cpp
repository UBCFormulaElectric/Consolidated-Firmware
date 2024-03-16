#include <pybind11/pybind11.h>
#include <stdlib.h>
#include "logfs.h"

namespace py = pybind11;

static LogFsErr readWrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
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

static LogFsErr writeWrapper(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    // Invoke user-defined write function.
    py::object *context = (py::object *)cfg->context;
    py::bytes   bytes   = py::bytes((char *)buf, cfg->block_size);
    py::object  result  = context->attr("write")(block, bytes);
    return result.cast<LogFsErr>();
}

class PyLogFs
{
  public:
    PyLogFs(uint32_t block_size, uint32_t block_count, py::object context) : context(context)
    {
        // Init config struct.
        cfg.block_size  = block_size;
        cfg.block_count = block_count;
        cfg.context     = &this->context;
        cfg.read        = readWrapper;
        cfg.write       = writeWrapper;

        // Allocate block cache on heap.
        cfg.cache = malloc(block_size);
    };

    ~PyLogFs()
    {
        // Deallocate block cache.
        free(cfg.cache);
    }

    LogFsErr mount(void) { return logfs_mount(&fs, &cfg); }

    LogFsErr format(void) { return logfs_format(&fs, &cfg); }

    LogFsErr open(LogFsFile &file, char *path)
    {
        // Allocate file cache on the heap.
        LogFsFileCfg file_cfg = {
            .cache = malloc(cfg.block_size),
            .path  = path,
        };
        return logfs_open(&fs, &file, &file_cfg);
    }

    LogFsErr close(LogFsFile &file)
    {
        // Deallocate file cache.
        const LogFsErr err = logfs_close(&fs, &file);
        if (err == LOGFS_ERR_OK)
        {
            // Only free file cache if close is successful.
            free(file.cache_data);
        }

        return err;
    }

    LogFsErr sync(LogFsFile &file) { return logfs_sync(&fs, &file); }

    py::tuple write(LogFsFile &file, const py::bytes bytes, uint32_t size)
    {
        // Write to disk.
        const std::string buf = bytes.cast<std::string>();
        uint32_t          num_written;
        const LogFsErr    err = logfs_write(&fs, &file, (void *)buf.data(), size, &num_written);

        // Return a tuple of (error, write size).
        return py::make_tuple(err, num_written);
    }

    py::tuple read(LogFsFile &file, uint32_t size, LogFsReadMode mode)
    {
        // Create an empty string to hold the read data.
        std::string    buf(size, '\0');
        uint32_t       num_read;
        const LogFsErr err = logfs_read(&fs, &file, (void *)buf.data(), size, mode, &num_read);

        // Return a tuple of (error, read size, read bytes).
        py::bytes bytes = py::bytes(buf);
        return py::make_tuple(err, num_read, bytes);
    }

    py::tuple firstPath(void)
    {
        // Get the first file's path.
        LogFsPath         path;
        const LogFsErr    err = logfs_firstPath(&fs, &path);
        const std::string path_str(path.path);

        // Return a tuple of (error, path, path string).
        return py::make_tuple(err, path, path_str);
    }

    py::tuple nextPath(LogFsPath path)
    {
        // Get the next file's path.
        const LogFsErr    err = logfs_nextPath(&fs, &path);
        const std::string path_str(path.path);

        // Return a tuple of (error, path, path string).
        return py::make_tuple(err, path, path_str);
    }

  private:
    LogFsCfg   cfg;
    LogFs      fs;
    py::object context;
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
        .value("UNIMPLEMENTED", LOGFS_ERR_UNIMPLEMENTED)
        .export_values();

    py::enum_<LogFsReadMode>(m, "PyLogFsReadMode")
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
        .def("open", &PyLogFs::open)
        .def("close", &PyLogFs::close)
        .def("sync", &PyLogFs::sync)
        .def("write", &PyLogFs::write)
        .def("read", &PyLogFs::read)
        .def("first_path", &PyLogFs::firstPath)
        .def("next_path", &PyLogFs::nextPath);
    // clang-format on
}
