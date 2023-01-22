#include <stdexcept>
#include <string>

#include <spdlog/spdlog.h>
#include <physfs.h>

#include "fs.h"

namespace OBR
{

FileSystem::FileSystem(int argc, char** argv)
{
	if (argc < 1 || PHYSFS_init(argv[0]) == 0)
		throw std::runtime_error("Could not initialize file system: " + std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));

	if (PHYSFS_mount("Data/", "Data/", 0) == 0)
		throw std::runtime_error("Could not mount the data folder: " + std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
	if (PHYSFS_mount("Carz.res", nullptr, 1) == 0) // Add ZRes to the end of search list
		spdlog::warn("Didn't manage to find a valid Carz.res file from the original Big Rigs installation. Some textures might appear incorrect.");

	for (int i = 1; i < argc; i++)
	{
		const char* path = argv[i];
		if (PHYSFS_mount(path, nullptr, 0) == 0)
			spdlog::error("Failed to load mod \"{}\": {}", path, std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
	}
}

FileSystem::~FileSystem()
{
	if (PHYSFS_deinit() == 0)
		spdlog::error("Failed to shutdown file system: " + std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
}

}
