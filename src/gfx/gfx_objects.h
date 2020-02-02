#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include <class_features/class_features.hpp>
#include <gfx/gfx_def.h>
#include <bgfx/c99/bgfx.h>

namespace pg::gfx
{

////////////////////////////////////////////////
class ShaderProgram 
    : public pg::class_features::NonCopyable
{
public:
    ShaderProgram();
    ShaderProgram(std::string const& vertexShaderFile, std::string const& fragmentShaderFile);

    ShaderProgram(ShaderProgram&& rhs);
    ShaderProgram& operator=(ShaderProgram&& rhs);

    ~ShaderProgram();

    bgfx_program_handle_t GetHandle() const;
    void Reload();


private:
    std::string m_VertexShaderFileName;
    std::string m_FragmentShaderFileName;

    std::shared_ptr<bgfx_shader_handle_t> m_VertexShaderHandle;
    std::shared_ptr<bgfx_shader_handle_t> m_FragmentShaderHandle;

    bgfx_program_handle_t m_ProgramHandle;
};

using SharedShaderProgram = std::shared_ptr<ShaderProgram>;


////////////////////////////////////////////////
class Texture : public pg::class_features::NonCopyable
{
public:
    Texture(TextureUsage usage, std::uint32_t width, std::uint32_t height, bgfx_texture_format format);
    Texture(std::string const& textureFile);

    Texture(Texture&& rhs);
    Texture& operator=(Texture&& rhs);

    ~Texture();

    bgfx_texture_handle_t GetHandle() const;

    TextureUsage        GetUsage() const;
    std::uint32_t       GetWidth() const;
    std::uint32_t       GetHeight() const;

private:
    bgfx_texture_handle_t m_TextureHandle;

    TextureUsage  m_Usage;
    std::uint32_t m_Width;
    std::uint32_t m_Height;
};

using SharedTexture = std::shared_ptr<Texture>;

////////////////////////////////////////////////

class VertexLayout : public pg::class_features::NonCopyable
{
public:
    VertexLayout();

    VertexLayout(VertexLayout&& rhs);
    VertexLayout& operator=(VertexLayout&& rhs);

    ~VertexLayout();

    bgfx_vertex_layout_t const* GetHandle() const;
    void AddAtribute(bgfx_attrib_t attributeSemantics, uint8_t elementsCount, bgfx_attrib_type_t dataType, bool normalize);
    void Bake();

private:
    bgfx_vertex_layout_t    m_VertexLayout;
    bool                    m_Started   : 1;
    bool                    m_Baked     : 1;
};

using SharedVertexLayout = std::shared_ptr<VertexLayout>;


////////////////////////////////////////////////

class VertexBuffer : public pg::class_features::NonCopyable
{
public:
    VertexBuffer();
    VertexBuffer(SharedVertexLayout const& layout, std::uint32_t vertexCount, void* buffer, std::uint32_t bytesCount);
    VertexBuffer(SharedVertexLayout const& layout, std::uint32_t vertexCount, bgfx_memory_t const* mem);
    
    VertexBuffer(VertexBuffer&& rhs);
    VertexBuffer& operator=(VertexBuffer&& rhs);

    ~VertexBuffer();

    bgfx_vertex_buffer_handle_t GetHandle() const;
    std::uint32_t GetVertexCount() const;

private:
    std::uint32_t               m_VertexCount;
    bgfx_vertex_buffer_handle_t m_VertexBufferHandle;
    SharedVertexLayout          m_Layout;

};

using SharedVertexBuffer = std::shared_ptr<VertexBuffer>;


////////////////////////////////////////////////

class IndexBuffer : public pg::class_features::NonCopyable
{
public:
    IndexBuffer();
    IndexBuffer(std::uint32_t indexCount, std::uint32_t* data);
    
    IndexBuffer(IndexBuffer&& rhs);
    IndexBuffer& operator=(IndexBuffer&& rhs);

    ~IndexBuffer();

    bgfx_index_buffer_handle_t GetHandle() const;
    std::uint32_t GetIndexCount() const;

private:
    std::uint32_t              m_IndexCount;
    bgfx_index_buffer_handle_t m_IndexBufferHandle;
};

using SharedIndexBuffer = std::shared_ptr<IndexBuffer>;



////////////////////////////////////////////////

class Framebuffer : public pg::class_features::NonCopyable
{
public:
    Framebuffer();

    Framebuffer(std::uint32_t attachment, bgfx_attachment_t* attachments);
    
    Framebuffer(Framebuffer&& rhs);
    Framebuffer& operator=(Framebuffer&& rhs);

    ~Framebuffer();

    bgfx_frame_buffer_handle_t GetHandle() const;

private:
    bgfx_frame_buffer_handle_t m_FramebufferHandle;
};

using SharedFramebuffer = std::shared_ptr<Framebuffer>;



////////////////////////////////////////////////

class UniformProxy : public pg::class_features::NonCopyable
{
public: 
    UniformProxy();
    UniformProxy(std::string const& name, bgfx_uniform_type type, std::uint32_t elementsCount = 1, std::uint8_t slot = 0);

    UniformProxy(UniformProxy&& rhs);
    UniformProxy& operator=(UniformProxy&& rhs);

    ~UniformProxy();

    void SetData(void* data, std::uint32_t numberOfElements = UINT16_MAX);
    void SetTexture(Texture* texture, std::uint32_t settings);

private:
    bgfx_uniform_handle_t   m_UniformHandle;
    bgfx_uniform_info_t     m_MetaData;
    std::uint8_t            m_Slot;

};

using SharedUniformProxy = std::shared_ptr<UniformProxy>;



////////////////////////////////////////////////
// UniformRawDataGeneric
template<std::uint32_t C_MAX_DATA_SIZE>
class UniformRawDataGeneric
{
public:
    UniformRawDataGeneric()
        : m_LastSize{ 0 }
        , m_Data{ 0 }
    {}

    void UpdateData(void const* data, std::uint32_t size)
    {
        assert(size <= C_MAX_DATA_SIZE && "Exceeded max UniformData size");
        std::memcpy(m_Data, data, size);
        m_LastSize = size;
    }

    template<typename T>
    void UpdateData(T const& data)
    {
        UpdateData(&data, static_cast<std::uint32_t>(sizeof(T)));
    }

    void SendData(UniformProxy& proxy)
    {
        if (m_LastSize == 0)
        {
            std::cout << "WARNING: Uniform raw data was never set." << std::endl;
        }
        else
        {
            proxy.SetData(m_Data, m_LastSize);
        }
    }

private:
    std::uint32_t m_LastSize;
    std::uint8_t  m_Data[C_MAX_DATA_SIZE];

};

using UniformRawData = UniformRawDataGeneric<128u>;


////////////////////////////////////////////////
// UniformTextureData
class UniformTextureData
{
public:
    UniformTextureData();
    UniformTextureData(SharedTexture const& texture);

    void SendData(UniformProxy& uniformProxy);

private:
   SharedTexture m_SharedTexture;
};

}

