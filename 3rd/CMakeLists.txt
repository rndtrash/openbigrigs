add_subdirectory(SDL)
add_subdirectory(SDL_mixer)
add_subdirectory(SDL_image)

set(BGFX_BUILD_EXAMPLES OFF)
add_subdirectory(bgfx.cmake)

add_subdirectory(spdlog)

set(PHYSFS_ARCHIVE_GRP OFF)
set(PHYSFS_ARCHIVE_WAD OFF)
set(PHYSFS_ARCHIVE_HOG OFF)
set(PHYSFS_ARCHIVE_MVL OFF)
set(PHYSFS_ARCHIVE_QPAK OFF)
set(PHYSFS_ARCHIVE_SLB OFF)
set(PHYSFS_ARCHIVE_ISO9660 OFF)
set(PHYSFS_ARCHIVE_VDF OFF)
set(PHYSFS_BUILD_SHARED OFF)
add_subdirectory(physfs)
