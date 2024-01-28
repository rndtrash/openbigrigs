#include <utility>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "resource.h"

namespace OBR {
    Resource::Resource(std::string path) : path(std::move(path)) {
        fileHandle = PHYSFS_openRead(this->path.c_str());
        if (fileHandle == nullptr)
            throw std::runtime_error("Could not open file " + this->path + ": " +
                                     std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }

    Resource::~Resource() {
        if (PHYSFS_close(this->fileHandle) == 0)
            spdlog::warn("Could not close file {}: {}", path,
                         std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }

    ssize_t Resource::get_file_size() {
        return PHYSFS_fileLength(this->fileHandle);
    }

    void Resource::read_file_to(const bgfx::Memory *memory) {
        assert(memory != nullptr);

        read_file_to(memory->data, memory->size);
    }

    void Resource::read_file_to(std::vector<uint8_t> &memory) {
        read_file_to(memory.data(), memory.size());
    }

    void Resource::read_file_to(uint8_t *memory, ssize_t size) {
        assert(memory != nullptr);

        const auto fileSize = get_file_size();
        if (size < fileSize)
            throw std::runtime_error(spdlog::fmt_lib::format(
                    "Not enough space to fit the file ({} bytes vs {})", fileSize, size));

        const auto bytesRead = PHYSFS_readBytes(this->fileHandle, memory, fileSize);
        if (bytesRead != fileSize)
            throw std::runtime_error(spdlog::fmt_lib::format(
                    "Failed to read the whole file ({} out of {} bytes)", bytesRead, fileSize));
    }
}
