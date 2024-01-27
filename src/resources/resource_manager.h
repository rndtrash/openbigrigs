#pragma once

#include "base_resource_manager.h"
#include "file.h"
#include "shader.h"

namespace OBR {
    class ResourceManager {
    public:
        ResourceManager(int argc, char **argv);

        ~ResourceManager();

        std::shared_ptr<BaseResourceManager<File>> files;
        std::shared_ptr<BaseResourceManager<Shader>> shaders;
    };
}
