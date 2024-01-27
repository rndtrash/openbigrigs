#include <physfs.h>
#include <spdlog/spdlog.h>

#include "resource_manager.h"

namespace OBR {

    ResourceManager::ResourceManager(int argc, char **argv) {
        // TODO: needs some changes for the Android build to work
        if (argc < 1 || PHYSFS_init(argv[0]) == 0)
            throw std::runtime_error("Could not initialize file system: " +
                                     std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));

        if (PHYSFS_mount("Data/", "Data/", 0) == 0)
            spdlog::warn("Could not mount the data folder: " +
                         std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
        if (PHYSFS_mount("Carz.res", nullptr, 1) == 0) // Add ZRes to the end of search list
            spdlog::warn(
                    "Didn't manage to find a valid Carz.res file from the original Big Rigs installation. Some textures might appear incorrect.");

        for (int i = 1; i < argc; i++) {
            const char *path = argv[i];
            if (PHYSFS_mount(path, nullptr, 0) == 0)
                spdlog::error("Failed to load mod \"{}\": {}", path,
                              std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
        }

        files = std::make_shared<BaseResourceManager<File>>();
        shaders = std::make_shared<BaseResourceManager<Shader>>();
    }

    ResourceManager::~ResourceManager() {
        files.reset();
        shaders.reset();

        if (PHYSFS_deinit() == 0)
            spdlog::error(
                    "Failed to shutdown file system: " + std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
    }
}