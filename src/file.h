#pragma once

#include <string>
#include <physfs.h>
#include <bgfx/bgfx.h>

namespace OBR {
    class File {
    public:
        explicit File() = delete;

        explicit File(std::string path);

        ~File();

        ssize_t get_size();

        void read_to(const bgfx::Memory *memory);

    private:
        std::string path;
        PHYSFS_file *fileHandle;
    };
}