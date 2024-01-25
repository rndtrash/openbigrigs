cmake_minimum_required(VERSION 3.16)

# Settings
# option(ZRES_FILENAME "Filename of the .res resource pack" "Carz.res")

# Include modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# OpenBigRigs
project(OpenBigRigs VERSION 0.0.0)

# Add 3rd party libraries
add_subdirectory(3rd)

# Add main target
add_compile_options(-Wall -Wextra -Werror -Wpedantic -Wno-error=unused-but-set-parameter -Wno-error=pedantic)
add_executable(OpenBigRigs "")

target_include_directories(OpenBigRigs
        PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
)

target_link_libraries(OpenBigRigs
        PRIVATE
        ${SDL2_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        bx
        bgfx
        spdlog
        PhysFS::PhysFS-static
)

add_subdirectory(assets)
add_subdirectory(src)
