#include <bx/math.h>
#include <bx/string.h>
#include <bgfx/bgfx.h>
#include <physfs.h>
#include <spdlog/spdlog.h>

#include "renderer.h"
#include "game.h"
#include "resources/base_resource_manager.h"

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
    std::shared_ptr<Shader> vsh;
    std::shared_ptr<Shader> fsh;
    static bgfx::ProgramHandle program;

    Renderer::Renderer() {
        window = new ObrWindow("OpenBigRigs", 800, 600);

        vertexLayout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

        vertexBufferHandle = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
                                                      vertexLayout);
        indexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

        vsh = Game::the().get_rm()->shaders->get("vs_cubes");
        fsh = Game::the().get_rm()->shaders->get("fs_cubes");
        program = bgfx::createProgram(vsh->get_handle(), fsh->get_handle(), false);
    }

    Renderer::~Renderer() {
        destroy(indexBufferHandle);
        destroy(vertexBufferHandle);
        vsh.reset();
        fsh.reset();
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