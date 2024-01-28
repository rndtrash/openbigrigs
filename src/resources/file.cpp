#include "file.h"

namespace OBR {
    File::File(const std::string &path) : Resource(path) {
    }

    ssize_t File::getSize() {
        return get_file_size();
    }

    void File::readTo(const bgfx::Memory *memory) {
        read_file_to(memory);
    }

    void File::readTo(std::vector<uint8_t> &memory) {
        read_file_to(memory);
    }

    void File::readTo(uint8_t *memory, ssize_t size) {
        read_file_to(memory, size);
    }
}