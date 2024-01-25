#
# Add sources
#

set(OBR_HEADERS
        audio.h
        window.h
        fs.h
        renderer.h
        game.h
)

set(OBR_SOURCES
        audio.cpp
        window.cpp
        fs.cpp
        renderer.cpp
        game.cpp

        main.cpp
)

target_sources(OpenBigRigs
        PRIVATE
        ${OBR_SOURCES}
        ${OBR_HEADERS}
)

target_compile_features(OpenBigRigs
        PRIVATE
        cxx_std_20
)

target_include_directories(OpenBigRigs
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
