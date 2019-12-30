#include <bgfx_helpers.h>
#include <gfx/gfx_objects.h>

#include <stb/stb_image.h>

namespace pg::gfx
{

ShaderProgram::ShaderProgram()
    : m_VertexShaderHandle{ nullptr }
    , m_FragmentShaderHandle{ nullptr }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{

}

ShaderProgram::ShaderProgram(std::string const& vertexShaderFile, std::string const& fragmentShaderFile)
    : m_VertexShaderHandle{ bgfx_helpers::makeSharedShader(vertexShaderFile) }
    , m_FragmentShaderHandle{ bgfx_helpers::makeSharedShader(fragmentShaderFile) }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    m_ProgramHandle = bgfx_create_program(*m_VertexShaderHandle, *m_FragmentShaderHandle, false);
}

ShaderProgram::ShaderProgram(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader)
    : m_VertexShaderHandle{ vertexShader }
    , m_FragmentShaderHandle{ fragmentShader }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    m_ProgramHandle = bgfx_create_program(*m_VertexShaderHandle, *m_FragmentShaderHandle, false);
}

ShaderProgram::ShaderProgram(ShaderProgram&& rhs)

ShaderProgram::~ShaderProgram()
{
    if (BGFX_HANDLE_IS_VALID(m_ProgramHandle))
    {
        bgfx_destroy_program(m_ProgramHandle);
        m_ProgramHandle = BGFX_INVALID_HANDLE;
    }
}

bgfx_program_handle_t ShaderProgram::GetHandle() const
{
    return m_ProgramHandle;
}


///////////////////////////////////////////////////////////////////////////
// Texture

Texture::Texture(std::string const& textureFileName)
    : m_Usage{ TextureUsage::Sampled }
    , m_TextureHandle{ BGFX_INVALID_HANDLE }
    , m_Width{ 0 }
    , m_Height{ 0 }
{
    int const componentsCount = 3;
    int width = 0, height = 0, components = 0;
    void* textureData = stbi_load(textureFileName.c_str(), &width, &height, &components, componentsCount);

    assert(width && height && components && "stbi failed to load texture.");

    m_Width = static_cast<std::uint32_t>(width);
    m_Height = static_cast<std::uint32_t>(height);

    bgfx_texture_format_t textureFormat = BGFX_TEXTURE_FORMAT_COUNT;
    switch (componentsCount)
    {
    case 4:
        textureFormat = BGFX_TEXTURE_FORMAT_RGBA8;
        break;
    case 3:
        textureFormat = BGFX_TEXTURE_FORMAT_RGB8;
        break;
    default:
        assert(false && "Only 3 and 4 component count is supported for textures!");
    }

    bgfx_release_fn_t stbiFreeDelegate = (bgfx_release_fn_t)[](void* memory, void* userData) { stbi_image_free(memory); };
    bgfx_memory_t const* textureDataBgfx = bgfx_make_ref_release(textureData, width * height * components, stbiFreeDelegate, nullptr);

    m_TextureHandle = bgfx_create_texture_2d(width, height, false, 1, textureFormat, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, textureDataBgfx);
}

Texture::Texture(TextureUsage usage, std::uint32_t width, std::uint32_t height, bgfx_texture_format format)
    : m_Usage{ usage }
    , m_TextureHandle{ BGFX_INVALID_HANDLE }
    , m_Width{ width }
    , m_Height{ height }
{
    std::uint64_t textureFlags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
    switch (m_Usage)
    {
    case gfx::TextureUsage::RenderTarget:
        textureFlags |= BGFX_TEXTURE_RT;
        break;
    }

    m_TextureHandle = bgfx_create_texture_2d(width, height, false, 1, format, textureFlags, nullptr);
}

Texture::~Texture()
{
    bgfx_destroy_texture(m_TextureHandle);
}

bgfx_texture_handle_t Texture::GetHandle() const
{
    return m_TextureHandle;
}

TextureUsage Texture::GetUsage() const
{
    return m_Usage;
}

std::uint32_t Texture::GetWidth() const
{
    return m_Width;
}

std::uint32_t Texture::GetHeight() const
{
    return m_Height;
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

