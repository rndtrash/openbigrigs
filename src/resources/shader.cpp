#include <sstream>

#include <spdlog/spdlog.h>
#include <bgfx/bgfx.h>

#include "shader.h"

namespace OBR {
    Shader::Shader(const std::string &name) : Resource(get_path_from_name(name)) {
        auto size = get_file_size();
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        read_file_to(mem);
        mem->data[mem->size - 1] = '\0';

        shaderHandle = bgfx::createShader(mem);
        bgfx::setName(shaderHandle, name.c_str());
    }

    std::string Shader::get_path_from_name(const std::string &name) {
        std::stringstream shaderPath{};

        switch (auto rt = bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                shaderPath << "Data/shaders/dx11/";
                break;
            case bgfx::RendererType::Agc:
            case bgfx::RendererType::Gnm:
                shaderPath << "Data/shaders/pssl/";
                break;
            case bgfx::RendererType::Metal:
                shaderPath << "Data/shaders/metal/";
                break;
            case bgfx::RendererType::Nvn:
                shaderPath << "Data/shaders/nvn/";
                break;
            case bgfx::RendererType::OpenGL:
                shaderPath << "Data/shaders/glsl/";
                break;
            case bgfx::RendererType::OpenGLES:
                shaderPath << "Data/shaders/essl/";
                break;
            case bgfx::RendererType::Vulkan:
                shaderPath << "Data/shaders/spirv/";
                break;

            default:
                throw std::runtime_error(
                        spdlog::fmt_lib::format("Invalid renderer type: {} ({})", (int) rt, getRendererName(rt)));
        }

        shaderPath << name << ".bin";

        return shaderPath.str();
    }

    Shader::~Shader() {
        destroy(shaderHandle);
    }
}