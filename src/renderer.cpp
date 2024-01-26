#include <bx/math.h>
#include <bx/string.h>
#include <bgfx/bgfx.h>
#include <physfs.h>
#include <spdlog/spdlog.h>

#include "renderer.h"
#include "game.h"
#include "fs.h"

namespace OBR {
    struct PosColorVertex {
        float x;
        float y;
        float z;
        uint32_t abgr;
    };

    static PosColorVertex s_cubeVertices[] =
            {
                    {-1.0f, 1.0f,  1.0f,  0xff000000},
                    {1.0f,  1.0f,  1.0f,  0xff0000ff},
                    {-1.0f, -1.0f, 1.0f,  0xff00ff00},
                    {1.0f,  -1.0f, 1.0f,  0xff00ffff},
                    {-1.0f, 1.0f,  -1.0f, 0xffff0000},
                    {1.0f,  1.0f,  -1.0f, 0xffff00ff},
                    {-1.0f, -1.0f, -1.0f, 0xffffff00},
                    {1.0f,  -1.0f, -1.0f, 0xffffffff},
            };

    static const uint16_t s_cubeTriList[] =
            {
                    0, 1, 2, // 0
                    1, 3, 2,
                    4, 6, 5, // 2
                    5, 6, 7,
                    0, 2, 4, // 4
                    4, 2, 6,
                    1, 5, 3, // 6
                    5, 7, 3,
                    0, 4, 1, // 8
                    4, 5, 1,
                    2, 3, 6, // 10
                    6, 3, 7,
            };

    static bgfx::VertexLayout vertexLayout;
    static bgfx::VertexBufferHandle vertexBufferHandle;
    static bgfx::IndexBufferHandle indexBufferHandle;
    static bgfx::ShaderHandle vsh;
    static bgfx::ShaderHandle fsh;
    static bgfx::ProgramHandle program;

    static const bgfx::Memory *loadMem(const char *_filePath) {
        // TODO: move this to the filesystem
        auto fs = Game::the().get_fs();
        auto file = fs->get_file(_filePath);
        auto size = file->get_size();
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        file->read_to(mem);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    static bgfx::ShaderHandle loadShader(const char *_name) {
        char filePath[512];

        const char *shaderPath = "???";

        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                shaderPath = "Data/shaders/dx11/";
                break;
            case bgfx::RendererType::Agc:
            case bgfx::RendererType::Gnm:
                shaderPath = "Data/shaders/pssl/";
                break;
            case bgfx::RendererType::Metal:
                shaderPath = "Data/shaders/metal/";
                break;
            case bgfx::RendererType::Nvn:
                shaderPath = "Data/shaders/nvn/";
                break;
            case bgfx::RendererType::OpenGL:
                shaderPath = "Data/shaders/glsl/";
                break;
            case bgfx::RendererType::OpenGLES:
                shaderPath = "Data/shaders/essl/";
                break;
            case bgfx::RendererType::Vulkan:
                shaderPath = "Data/shaders/spirv/";
                break;

            case bgfx::RendererType::Count:
                BX_ASSERT(false, "You should not be here!");
                break;
        }

        bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
        bx::strCat(filePath, BX_COUNTOF(filePath), _name);
        bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

        bgfx::ShaderHandle handle = bgfx::createShader(loadMem(filePath));
        bgfx::setName(handle, _name);

        return handle;
    }

    Renderer::Renderer() {
        window = new Window("OpenBigRigs", 800, 600);

        vertexLayout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

        vertexBufferHandle = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
                                                      vertexLayout);
        indexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

        vsh = loadShader("vs_cubes");
        fsh = loadShader("fs_cubes");
        program = bgfx::createProgram(vsh, fsh, true);
    }

    Renderer::~Renderer() {
        destroy(indexBufferHandle);
        destroy(vertexBufferHandle);
        destroy(vsh);
        destroy(fsh);
        destroy(program);

        delete window;
        bgfx::shutdown();
    }

    void Renderer::render() {
        auto window_size = window->get_size();

        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(window_size.first) / float(window_size.second), 0.1f, 100.0f,
                    bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        bgfx::setVertexBuffer(0, vertexBufferHandle);
        bgfx::setIndexBuffer(indexBufferHandle);

        bgfx::setViewTransform(0, view, proj);
        float mtx[16];
        const auto ticks = float(SDL_GetTicks());
        bx::mtxRotateXY(mtx, ticks * 0.01f, ticks * 0.01f);
        bgfx::setTransform(mtx);

        bgfx::submit(0, program);

//        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
//        const bx::Vec3 eye = {0.0f, 0.0f, -1.0f};
//
//        float view[16];
//        bx::mtxLookAt(view, eye, at);
//
//        // Setup a top-left ortho matrix for screen space drawing.
//        const bgfx::Caps *caps = bgfx::getCaps();
//        {
//
//            float ortho[16];
//            bx::mtxOrtho(
//                    ortho, 0.0f, float(window_size.first), float(window_size.second), 0.0f, 0.0f, 100.0f, 0.0f,
//                    caps->homogeneousDepth
//            );
//            bgfx::setViewTransform(0, view, ortho);
//            bgfx::setViewRect(0, 0, 0, uint16_t(window_size.first), uint16_t(window_size.second));
//        }
    }

}