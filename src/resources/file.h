#pragma once

#include "resources/resource.h"

namespace OBR {
    class File : public Resource {
    public:
        explicit File(const std::string &path);

        ssize_t getSize();

        void readTo(const bgfx::Memory *memory);

        void readTo(std::vector<uint8_t> &memory);

        void readTo(uint8_t *memory, ssize_t size);
    };
}