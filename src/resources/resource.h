#pragma once

#include <string>
#include <vector>

#include <physfs.h>
#include <bgfx/bgfx.h>

namespace OBR {
    class Resource {
    public:
        explicit Resource(std::string path);

        virtual ~Resource();

        std::string get_path() { return path; }

    protected:
        ssize_t get_file_size();

        void read_file_to(const bgfx::Memory *memory);

        void read_file_to(std::vector<uint8_t> &memory);

        void read_file_to(uint8_t *memory, ssize_t size);

        std::string path;

    private:
        PHYSFS_file *fileHandle;
    };

    template<typename T>
    concept DerivedFromResource = std::is_base_of_v<Resource, T>;
}