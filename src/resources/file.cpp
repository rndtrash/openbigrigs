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
}