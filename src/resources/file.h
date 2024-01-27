#pragma once

#include "resources/resource.h"

namespace OBR {
    class File : public Resource {
    public:
        explicit File(const std::string &path);

        ssize_t getSize();

        void readTo(const bgfx::Memory *memory);
    };
}