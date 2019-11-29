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

TextureRef::TextureRef(std::string const& textureFileName)
    : m_TextureHandle{ nullptr }
    , m_Width{ 0 }
    , m_Height{ 0 }
{
    int width = 0, height = 0;
    m_TextureHandle = bgfx_helpers::makeShared2DTexture(textureFileName, 3, &width, &height);

    m_Width     = static_cast<std::uint32_t>(width);
    m_Height    = static_cast<std::uint32_t>(height);
}

TextureRef::TextureRef(std::uint32_t width, std::uint32_t height, bgfx_texture_format format, TextureUsage usage)
    : m_TextureHandle{ bgfx_helpers::makeShared2DTexture(width, height, format, usage) }
    , m_Width{ width }
    , m_Height{ height }
{
}

bgfx_texture_handle_t TextureRef::GetHandle() const
{
    return m_TextureHandle ? *m_TextureHandle : bgfx_texture_handle_t{ BGFX_INVALID_HANDLE };
}


///////////////////////////////////////////////////////////////////////////
// VertexBufferRef

VertexBufferRef::VertexBufferRef(std::uint32_t vertexCount, bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout)
    : m_VertexBufferHandle{ bgfx_helpers::makeSharedVertexBuffer(mem, layout) }
    , m_VertexCount{ vertexCount }
{}

VertexBufferRef::~VertexBufferRef() = default;

bgfx_vertex_buffer_handle_t VertexBufferRef::GetHandle() const
{
    return *m_VertexBufferHandle;
}

std::uint32_t VertexBufferRef::GetVertexCount() const
{
    return m_VertexCount;
}


}

