#pragma once

#include "bgfx/bgfx.h"

#include "resource.h"

namespace OBR {
    class Shader : public Resource {
    public:
        explicit Shader(const std::string &name);

        ~Shader() override;

        static std::string get_path_from_name(const std::string &name);

        bgfx::ShaderHandle get_handle() { return shaderHandle; }

    private:
        bgfx::ShaderHandle shaderHandle;
    };
}
