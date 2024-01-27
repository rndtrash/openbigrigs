#pragma once

#include <string>
#include "physfs.h"
#include "bgfx/bgfx.h"

namespace OBR {
    class Resource {
    public:
        explicit Resource(std::string path);

        virtual ~Resource();

        std::string get_path() { return path; }

    protected:
        ssize_t get_file_size();

        void read_file_to(const bgfx::Memory *memory);

        std::string path;

    private:
        PHYSFS_file *fileHandle;
    };

    template<typename T>
    concept DerivedFromResource = std::is_base_of_v<Resource, T>;
}