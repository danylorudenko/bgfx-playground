#include <gfx/gfx_objects.h>
#include <memory_helpers.h>

namespace pg::gfx
{

Shader::Shader(bgfx_program_handle_t handle)
    : m_ProgramHandle{ handle }
    , m_VertexShaderHandle{ nullptr }
    , m_FragmentShaderHandle{ nullptr }
{

}

Shader::Shader(std::string const& vertexShaderFile, std::string const& fragmentShaderFile)
    : m_VertexShaderHandle{ memory_helpers::makeSharedShader(vertexShaderFile) }
    , m_FragmentShaderHandle{ memory_helpers::makeSharedShader(fragmentShaderFile) }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    memory_helpers::makeSharedProgram(*m_VertexShaderHandle, *m_FragmentShaderHandle);
}

Shader::Shader(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader)
{

}

Shader::~Shader()
{
    if (BGFX_HANDLE_IS_VALID(m_ProgramHandle))
    {
        bgfx_destroy_program(m_ProgramHandle);
        m_ProgramHandle = BGFX_INVALID_HANDLE;
    }
    
}



}

