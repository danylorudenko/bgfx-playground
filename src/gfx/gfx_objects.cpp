#include <gfx/gfx_objects.h>
#include <bgfx_helpers.h>

namespace pg::gfx
{

ShaderRef::ShaderRef(bgfx_program_handle_t handle)
    : m_VertexShaderHandle{ nullptr }
    , m_FragmentShaderHandle{ nullptr }
    , m_ProgramHandle{ std::make_shared<bgfx_program_handle_t>(handle) }
{

}

ShaderRef::ShaderRef(std::string const& vertexShaderFile, std::string const& fragmentShaderFile)
    : m_VertexShaderHandle{ bgfx_helpers::makeSharedShader(vertexShaderFile) }
    , m_FragmentShaderHandle{ bgfx_helpers::makeSharedShader(fragmentShaderFile) }
    , m_ProgramHandle{ bgfx_helpers::makeSharedProgram(*m_VertexShaderHandle, *m_FragmentShaderHandle) }
{
}

ShaderRef::ShaderRef(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader)
    : m_VertexShaderHandle{ vertexShader }
    , m_FragmentShaderHandle{ fragmentShader }
    , m_ProgramHandle{ bgfx_helpers::makeSharedProgram(*m_VertexShaderHandle, *m_FragmentShaderHandle) }
{
}

bgfx_program_handle_t ShaderRef::GetHandle() const
{
    return m_ProgramHandle ? *m_ProgramHandle : bgfx_program_handle_t{ BGFX_INVALID_HANDLE };
}


///////////////////////////////////////////////////////////////////////////
// TextureRef

TextureRef::TextureRef(bgfx_texture_handle_t handle)
    : m_TextureHandle{ std::make_shared<bgfx_texture_handle_t>(handle) }
{}

TextureRef::TextureRef(std::string const& textureFileName)
    : m_TextureHandle{ bgfx_helpers::makeShared2DTexture(textureFileName) }
{}

TextureRef::TextureRef(std::uint32_t width, std::uint32_t height, bgfx_texture_format format, TextureUsage usage)
    : m_TextureHandle{ bgfx_helpers::makeShared2DTexture(width, height, format, usage) }
{
}

bgfx_texture_handle_t TextureRef::GetHandle() const
{
    return m_TextureHandle ? *m_TextureHandle : bgfx_texture_handle_t{ BGFX_INVALID_HANDLE };
}

}

