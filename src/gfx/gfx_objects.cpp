#include <gfx/gfx_objects.h>

#include <cassert>

#include <gfx/bgfx_helpers.h>
#include <gfx/gfx_renderer.h>
#include <stb/stb_image.h>

namespace pg::gfx
{

ShaderProgram::ShaderProgram()
    : m_VertexShaderHandle{ nullptr }
    , m_FragmentShaderHandle{ nullptr }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    Renderer::GetInstance()->RegisterShader(this);
}

ShaderProgram::ShaderProgram(std::string const& vertexShaderFile, std::string const& fragmentShaderFile)
    : m_VertexShaderFileName{ vertexShaderFile }
    , m_FragmentShaderFileName{ fragmentShaderFile }
    , m_VertexShaderHandle{ bgfx_helpers::makeSharedShader(vertexShaderFile) }
    , m_FragmentShaderHandle{ bgfx_helpers::makeSharedShader(fragmentShaderFile) }
    , m_ProgramHandle{ BGFX_INVALID_HANDLE }
{
    m_ProgramHandle = bgfx_create_program(*m_VertexShaderHandle, *m_FragmentShaderHandle, false);

    Renderer::GetInstance()->RegisterShader(this);
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
    std::swap(m_VertexShaderFileName, rhs.m_VertexShaderFileName);
    std::swap(m_FragmentShaderFileName, rhs.m_FragmentShaderFileName);
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
    }

    Renderer::GetInstance()->UnregisterShader(this);
}

bgfx_program_handle_t ShaderProgram::GetHandle() const
{
    return m_ProgramHandle;
}

void ShaderProgram::Reload()
{
    if (!BGFX_HANDLE_IS_VALID(m_ProgramHandle))
        return;
    
    m_VertexShaderHandle = bgfx_helpers::makeSharedShader(m_VertexShaderFileName);
    m_FragmentShaderHandle = bgfx_helpers::makeSharedShader(m_FragmentShaderFileName);

    bgfx_destroy_program(m_ProgramHandle);
    m_ProgramHandle = bgfx_create_program(*m_VertexShaderHandle, *m_FragmentShaderHandle, false);
    assert(BGFX_HANDLE_IS_VALID(m_ProgramHandle) && "Failed to reload shader!");
}


///////////////////////////////////////////////////////////////////////////
// Texture

Texture::Texture(std::string const& textureFileName)
    : m_TextureHandle{ BGFX_INVALID_HANDLE }
    , m_Usage{ TextureUsage::Sampled }
    , m_Width{ 0 }
    , m_Height{ 0 }
{
    int const forceComponentsCount = 3;
    int width = 0, height = 0, components = 0;
    void* textureData = stbi_load(textureFileName.c_str(), &width, &height, &components, forceComponentsCount);

    assert(width && height && components && "stbi failed to load texture.");

    m_Width = static_cast<std::uint32_t>(width);
    m_Height = static_cast<std::uint32_t>(height);

    bgfx_texture_format_t textureFormat = BGFX_TEXTURE_FORMAT_COUNT;
    switch (forceComponentsCount)
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
    bgfx_memory_t const* textureDataBgfx = bgfx_make_ref_release(textureData, width * height * forceComponentsCount, stbiFreeDelegate, nullptr);

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
    case gfx::TextureUsage::DepthReadWrite:
        textureFlags |= BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL;
        break;
    default:
        assert(false);
    }

    m_TextureHandle = bgfx_create_texture_2d(width, height, false, 1, format, textureFlags, nullptr);
    assert(BGFX_HANDLE_IS_VALID(m_TextureHandle) && "Failed to create a texture.");
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
    bgfx_memory_t const* memory = bgfx_copy(buffer, bytesCount);
    m_VertexBufferHandle = bgfx_create_vertex_buffer(memory, m_Layout->GetHandle(), BGFX_BUFFER_NONE);
}

VertexBuffer::~VertexBuffer()
{
    if (BGFX_HANDLE_IS_VALID(m_VertexBufferHandle))
    {
        bgfx_destroy_vertex_buffer(m_VertexBufferHandle);
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
// IndexBuffer
IndexBuffer::IndexBuffer()
    : m_IndexCount{ 0 }
    , m_IndexBufferHandle{ BGFX_INVALID_HANDLE }
{}

IndexBuffer::IndexBuffer(std::uint32_t indexCount, std::uint32_t* data)
    : m_IndexCount{ indexCount }
    , m_IndexBufferHandle{ BGFX_INVALID_HANDLE }
{
    bgfx_memory_t const* bgfxMemory = bgfx_copy(data, static_cast<std::uint32_t>(indexCount * sizeof(std::uint32_t)));

    m_IndexBufferHandle = bgfx_create_index_buffer(bgfxMemory, BGFX_BUFFER_INDEX32);
    assert(BGFX_HANDLE_IS_VALID(m_IndexBufferHandle) && "Failed to create index buffer.");
}

IndexBuffer::IndexBuffer(IndexBuffer&& rhs)
    : m_IndexCount{ 0 }
    , m_IndexBufferHandle{ BGFX_INVALID_HANDLE }
{
    operator=(std::move(rhs));
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& rhs)
{
    std::swap(m_IndexCount, rhs.m_IndexCount);
    std::swap(m_IndexBufferHandle, rhs.m_IndexBufferHandle);

    return *this;
}

IndexBuffer::~IndexBuffer()
{
    if (BGFX_HANDLE_IS_VALID(m_IndexBufferHandle))
    {
        bgfx_destroy_index_buffer(m_IndexBufferHandle);
    }
}

bgfx_index_buffer_handle_t IndexBuffer::GetHandle() const
{
    return m_IndexBufferHandle;
}

std::uint32_t IndexBuffer::GetIndexCount() const
{
    return m_IndexCount;
}


///////////////////////////////////////////////////////////////////////////
// Framebuffer
Framebuffer::Framebuffer()
    : m_FramebufferHandle{ BGFX_INVALID_HANDLE }
{
}

Framebuffer::Framebuffer(std::uint32_t attachmentCount, bgfx_attachment_t* attachments)
    : m_FramebufferHandle{ BGFX_INVALID_HANDLE }
{
    m_FramebufferHandle = bgfx_create_frame_buffer_from_attachment(attachmentCount, attachments, false);
    assert(BGFX_HANDLE_IS_VALID(m_FramebufferHandle) && "Failed to create Framebuffer.");
}

Framebuffer::Framebuffer(Framebuffer&& rhs)
    : m_FramebufferHandle{ BGFX_INVALID_HANDLE }
{
    operator=(std::move(rhs));
}

Framebuffer& Framebuffer::operator=(Framebuffer&& rhs)
{
    std::swap(m_FramebufferHandle, rhs.m_FramebufferHandle);

    return *this;
}

Framebuffer::~Framebuffer()
{
    if (BGFX_HANDLE_IS_VALID(m_FramebufferHandle))
    {
        bgfx_destroy_frame_buffer(m_FramebufferHandle);
    }
}

bgfx_frame_buffer_handle_t Framebuffer::GetHandle() const
{
    return m_FramebufferHandle;
}


///////////////////////////////////////////////////////////////////////////
// UniformProxy
UniformProxy::UniformProxy()
    : m_UniformHandle{ BGFX_INVALID_HANDLE }
    , m_MetaData{}
{}

UniformProxy::UniformProxy(std::string const& name, bgfx_uniform_type type, std::uint32_t elementsCount, std::uint8_t slot)
    : m_UniformHandle{ BGFX_INVALID_HANDLE }
    , m_MetaData{}
    , m_Slot{ slot }
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
    m_Slot = rhs.m_Slot;

    return *this;
}

UniformProxy::~UniformProxy()
{
    if (BGFX_HANDLE_IS_VALID(m_UniformHandle))
    {
        bgfx_destroy_uniform(m_UniformHandle);
    }
}

void UniformProxy::SetData(void* data, std::uint32_t elementsCount)
{
    assert(BGFX_HANDLE_IS_VALID(m_UniformHandle) && "Attempt to set data to invalid uniform");
    assert(m_MetaData.type != BGFX_UNIFORM_TYPE_SAMPLER && "Attempt to set binary data to a sampler uniform.");

    bgfx_set_uniform(m_UniformHandle, data, static_cast<std::uint16_t>(elementsCount));
}

void UniformProxy::SetTexture(Texture* texture, std::uint32_t settings)
{
    assert(BGFX_HANDLE_IS_VALID(m_UniformHandle) && "Attempt to set data to invalid uniform");
    assert(m_MetaData.type == BGFX_UNIFORM_TYPE_SAMPLER && "Attempt to set texture to a non-sampler uniform.");

    bgfx_set_texture(m_Slot, m_UniformHandle, texture->GetHandle(), settings);
}


///////////////////////////////////////////////////////////////////////////
// UniformTextureData
UniformTextureData::UniformTextureData() = default;

UniformTextureData::UniformTextureData(SharedTexture const& texture)
    : m_SharedTexture{ texture }
{}

void UniformTextureData::SendData(UniformProxy& uniformProxy)
{
    if (m_SharedTexture && BGFX_HANDLE_IS_VALID(m_SharedTexture->GetHandle()))
    {
        uniformProxy.SetTexture(m_SharedTexture.get(), BGFX_SAMPLER_UVW_MIRROR);
    }
    else
    {
        std::cout << "WARNING: uniform texture data was never set." << std::endl;
    }
}


}

