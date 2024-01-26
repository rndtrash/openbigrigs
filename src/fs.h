#pragma once

#include "file.h"

namespace OBR {

    class FileSystem {
    public:
        FileSystem(int argc, char **argv);

        ~FileSystem();

        std::unique_ptr<File> get_file(const std::string &path);

    private:
    };

}
