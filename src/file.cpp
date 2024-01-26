#include <utility>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "file.h"

namespace OBR {
    File::File(std::string path) : path(std::move(path)) {
        fileHandle = PHYSFS_openRead(this->path.c_str());
        if (fileHandle == nullptr)
            throw std::runtime_error("Could not open file " + this->path + ": " +
                                     std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }

    File::~File() {
        if (PHYSFS_close(this->fileHandle) == 0)
            spdlog::warn("Could not close file {}: {}", path,
                         std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }

    ssize_t File::get_size() {
        return PHYSFS_fileLength(this->fileHandle);
    }

    void File::read_to(const bgfx::Memory *memory) {
        assert(memory != nullptr);

        const auto size = get_size();
        if (memory->size < size)
            throw std::runtime_error(spdlog::fmt_lib::format(
                    "Not enough space to fit the file ({} bytes vs {})", size, memory->size));

        const auto bytesRead = PHYSFS_readBytes(this->fileHandle, memory->data, size);
        if (bytesRead != size)
            throw std::runtime_error(spdlog::fmt_lib::format(
                    "Failed to read the whole file ({} out of {} bytes)", bytesRead, size));
    }
}
