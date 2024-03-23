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

class PyLogFsFile
{
  public:
    PyLogFsFile(void) { file.is_open = false; }

    std::string path(void) { return file.path; }

    LogFsFile file;
};

class PyLogFs
{
  public:
    PyLogFs(uint32_t block_size, uint32_t block_count, uint32_t write_cycles, bool rd_only, py::object context)
      : context(context)
    {
        // Init config struct.
        cfg.block_size   = block_size;
        cfg.block_count  = block_count;
        cfg.write_cycles = write_cycles;
        cfg.rd_only      = rd_only;
        cfg.context      = &this->context;
        cfg.read         = readWrapper;
        cfg.write        = writeWrapper;

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

    LogFsErr open(PyLogFsFile &file, char *path, uint32_t flags)
    {
        // Allocate file cache on the heap.
        LogFsFileCfg file_cfg = {
            .cache = malloc(cfg.block_size),
            .path  = path,
        };
        return logfs_open(&fs, &file.file, &file_cfg, flags);
    }

    LogFsErr close(PyLogFsFile &file)
    {
        // Deallocate file cache.
        const LogFsErr err = logfs_close(&fs, &file.file);
        if (err == LOGFS_ERR_OK)
        {
            // Only free file cache if close is successful.
            free(file.file.cache_data);
        }

        return err;
    }

    LogFsErr sync(PyLogFsFile &file) { return logfs_sync(&fs, &file.file); }

    LogFsErr write(PyLogFsFile &file, const py::bytes bytes)
    {
        // Write to disk.
        const std::string buf = bytes.cast<std::string>();
        return logfs_write(&fs, &file.file, (void *)buf.data(), buf.size());
    }

    py::tuple read(PyLogFsFile &file, uint32_t size, LogFsReadFlags flags)
    {
        // Create an empty string to hold the read data.
        std::string    buf(size, '\0');
        uint32_t       num_read;
        const LogFsErr err = logfs_read(&fs, &file.file, (void *)buf.data(), size, flags, &num_read);

        // Return a tuple of (error, read size, read bytes).
        py::bytes bytes = py::bytes(buf);
        return py::make_tuple(err, num_read, bytes);
    }

    LogFsErr writeMetadata(PyLogFsFile &file, const py::bytes bytes)
    {
        // Write to metadata.
        const std::string buf = bytes.cast<std::string>();
        return logfs_writeMetadata(&fs, &file.file, (void *)buf.data(), buf.size());
    }

    py::tuple readMetadata(PyLogFsFile &file, uint32_t size)
    {
        // Create an empty string to hold the read data.
        std::string    buf(size, '\0');
        uint32_t       num_read;
        const LogFsErr err = logfs_readMetadata(&fs, &file.file, (void *)buf.data(), size, &num_read);

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
        .value("UNMOUNTED", LOGFS_ERR_UNMOUNTED)
        .value("NOMEM", LOGFS_ERR_NOMEM)
        .value("NOT_OPEN", LOGFS_ERR_NOT_OPEN)
        .value("RD_ONLY", LOGFS_ERR_RD_ONLY)
        .value("WR_ONLY", LOGFS_ERR_WR_ONLY)
        .value("DNE", LOGFS_ERR_DNE)
        .export_values();


    py::enum_<LogFsOpenFlags>(m, "PyLogFsOpenFlags")
        .value("RD_ONLY", LOGFS_OPEN_RD_ONLY)
        .value("WR_ONLY", LOGFS_OPEN_WR_ONLY)
        .value("RD_WR", LOGFS_OPEN_RD_WR)
        .value("CREATE", LOGFS_OPEN_CREATE)
        .export_values();

    py::enum_<LogFsReadFlags>(m, "PyLogFsReadFlags")
        .value("END", LOGFS_READ_END)
        .value("ITER", LOGFS_READ_ITER)
        .export_values();

    py::class_<PyLogFsFile>(m, "PyLogFsFile")
        .def(py::init<>())
        .def("path", &PyLogFsFile::path);

    py::class_<LogFsPath>(m, "PyLogFsPath")
        .def(py::init<>());

    py::class_<PyLogFs>(m, "PyLogFs")
        .def(py::init<uint32_t, uint32_t, uint32_t, bool, py::object&>())
        .def("mount", &PyLogFs::mount)
        .def("format", &PyLogFs::format)
        .def("open", &PyLogFs::open)
        .def("close", &PyLogFs::close)
        .def("sync", &PyLogFs::sync)
        .def("write", &PyLogFs::write)
        .def("read", &PyLogFs::read)
        .def("write_metadata", &PyLogFs::writeMetadata)
        .def("read_metadata", &PyLogFs::readMetadata)
        .def("first_path", &PyLogFs::firstPath)
        .def("next_path", &PyLogFs::nextPath);
    // clang-format on
}
