#pragma once

#include <bgfx/c99/bgfx.h>
#include <memory>
#include <string>

namespace pg::gfx
{

class Shader
{
public:
    Shader(bgfx_program_handle_t handle = BGFX_INVALID_HANDLE);
    Shader(std::string const& vertexShaderFile, std::string const& fragmentShaderFile);
    Shader(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader);
    ~Shader();

    bgfx_program_handle_t GetHandle() const;


private:
    bgfx_program_handle_t m_ProgramHandle;

    std::shared_ptr<bgfx_shader_handle_t> m_VertexShaderHandle;
    std::shared_ptr<bgfx_shader_handle_t> m_FragmentShaderHandle;
};

}

