#include <gfx/bgfx_helpers.h>
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
    : m_VertexShaderHandle{ nullptr }
    , m_FragmentShaderHandle{ nullptr }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    operator=(std::move(rhs));
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs)
{
    std::swap(m_VertexShaderHandle, rhs.m_VertexShaderHandle);
    std::swap(m_FragmentShaderHandle, rhs.m_FragmentShaderHandle);
    std::swap(m_ProgramHandle, rhs.m_ProgramHandle);

    return *this;
}

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
    : m_TextureHandle{ BGFX_INVALID_HANDLE }
    , m_Usage{ TextureUsage::Sampled }
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

Texture::Texture(Texture&& rhs)
    : m_TextureHandle{ BGFX_INVALID_HANDLE }
    , m_Usage{ TextureUsage::None }
    , m_Width{ 0 }
    , m_Height{ 0 }
{
    operator=(std::move(rhs));
}

Texture& Texture::operator=(Texture&& rhs)
{
    std::swap(m_TextureHandle, rhs.m_TextureHandle);
    std::swap(m_Usage, rhs.m_Usage);
    std::swap(m_Width, rhs.m_Width);
    std::swap(m_Height, rhs.m_Height);

    return *this;
}

Texture::~Texture()
{
    if (BGFX_HANDLE_IS_VALID(m_TextureHandle))
    {
        bgfx_destroy_texture(m_TextureHandle);
        m_TextureHandle = BGFX_INVALID_HANDLE;
    }
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
// VertexLayout

VertexLayout::VertexLayout()
    : m_VertexLayout{}
    , m_Started{ false }
    , m_Baked{ false }
{
}

VertexLayout::VertexLayout(VertexLayout&&) = default;

VertexLayout& VertexLayout::operator=(VertexLayout&&) = default;

VertexLayout::~VertexLayout() = default;

bgfx_vertex_layout_t const* VertexLayout::GetHandle() const
{
    return &m_VertexLayout;
}

void VertexLayout::AddAtribute(bgfx_attrib_t attributeSemantics, uint8_t elementsCount, bgfx_attrib_type_t dataType, bool normalize)
{
    if (m_Baked)
    {
        assert(false && "Attempt to add attribute to baked VertexLayout.");
        return;
    }

    bgfx_vertex_layout_t* layoutPtr = &m_VertexLayout;
    if (!m_Started)
    {
        bgfx_vertex_layout_begin(layoutPtr, bgfx_get_renderer_type());
        m_Started = true;
    }

    bgfx_vertex_layout_add(layoutPtr, attributeSemantics, elementsCount, dataType, normalize, false);
}

void VertexLayout::Bake()
{
    if (m_Baked)
    {
        assert(false && "Attempt to bake VertexLayout second time.");
        return;
    }

    bgfx_vertex_layout_end(&m_VertexLayout);
    m_Baked = true;
}


///////////////////////////////////////////////////////////////////////////
// VertexBuffer

VertexBuffer::VertexBuffer()
    : m_VertexCount{ 0 }
    , m_VertexBufferHandle{ BGFX_INVALID_HANDLE }
    , m_Layout{ nullptr }
{
}

VertexBuffer::VertexBuffer(VertexBuffer&& rhs)
    : m_VertexCount{ 0 }
    , m_VertexBufferHandle{ BGFX_INVALID_HANDLE }
    , m_Layout{ nullptr }
{
    operator=(std::move(rhs));
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs)
{
    std::swap(m_VertexCount, rhs.m_VertexCount);
    std::swap(m_VertexBufferHandle, rhs.m_VertexBufferHandle);
    std::swap(m_Layout, rhs.m_Layout);

    return *this;
}

VertexBuffer::VertexBuffer(SharedVertexLayout const& layout, std::uint32_t vertexCount, bgfx_memory_t const* mem)
    : m_VertexCount{ vertexCount }
    , m_VertexBufferHandle{ BGFX_INVALID_HANDLE }
    , m_Layout{ layout }
{
    m_VertexBufferHandle = bgfx_create_vertex_buffer(mem, m_Layout->GetHandle(), BGFX_BUFFER_NONE);
}

VertexBuffer::VertexBuffer(SharedVertexLayout const& layout, std::uint32_t vertexCount, void* buffer, std::uint32_t bytesCount)
    : m_VertexCount{ vertexCount }
    , m_VertexBufferHandle{ BGFX_INVALID_HANDLE }
    , m_Layout{ layout }
{
    bgfx_memory_t const* memory = bgfx_make_ref(buffer, bytesCount);
    m_VertexBufferHandle = bgfx_create_vertex_buffer(memory, m_Layout->GetHandle(), BGFX_BUFFER_NONE);
}

VertexBuffer::~VertexBuffer()
{
    if (BGFX_HANDLE_IS_VALID(m_VertexBufferHandle))
    {
        bgfx_destroy_vertex_buffer(m_VertexBufferHandle);
        m_VertexBufferHandle = BGFX_INVALID_HANDLE;
    }
}

bgfx_vertex_buffer_handle_t VertexBuffer::GetHandle() const
{
    return m_VertexBufferHandle;
}

std::uint32_t VertexBuffer::GetVertexCount() const
{
    return m_VertexCount;
}


///////////////////////////////////////////////////////////////////////////
// UniformProxy
UniformProxy::UniformProxy()
    : m_UniformHandle{ BGFX_INVALID_HANDLE }
    , m_MetaData{}
{}

UniformProxy::UniformProxy(std::string const& name, bgfx_uniform_type type, std::uint32_t elementsCount)
    : m_UniformHandle{ BGFX_INVALID_HANDLE }
    , m_MetaData{}
{
    m_UniformHandle = bgfx_create_uniform(name.c_str(), type, static_cast<std::uint16_t>(elementsCount));
    bgfx_get_uniform_info(m_UniformHandle, &m_MetaData);
}

UniformProxy::UniformProxy(UniformProxy&& rhs)
    : m_UniformHandle{ BGFX_INVALID_HANDLE }
    , m_MetaData{}
{
    operator=(std::move(rhs));
}

UniformProxy& UniformProxy::operator=(UniformProxy&& rhs)
{
    std::swap(m_UniformHandle, rhs.m_UniformHandle);
    m_MetaData = rhs.m_MetaData;

    return *this;
}

UniformProxy::~UniformProxy()
{
    if (BGFX_HANDLE_IS_VALID(m_UniformHandle))
    {
        bgfx_destroy_uniform(m_UniformHandle);
        m_UniformHandle = BGFX_INVALID_HANDLE;
    }
}

void UniformProxy::SetData(void* data, std::uint32_t elementsCount)
{
    assert(BGFX_HANDLE_IS_VALID(m_UniformHandle) && "Attempt to set data to invalid uniform");
    assert(m_MetaData.type != BGFX_UNIFORM_TYPE_SAMPLER && "Attempt to set binary data to a sampler uniform.");

    bgfx_set_uniform(m_UniformHandle, data, static_cast<std::uint16_t>(elementsCount));
}

void UniformProxy::SetTexture(Texture* texture, int settings)
{
    assert(BGFX_HANDLE_IS_VALID(m_UniformHandle) && "Attempt to set data to invalid uniform");
    assert(m_MetaData.type == BGFX_UNIFORM_TYPE_SAMPLER && "Attempt to set texture to a non-sampler uniform.");

    bgfx_set_texture(0, m_UniformHandle, texture->GetHandle(), BGFX_SAMPLER_UVW_CLAMP);
}

}

