#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <gfx/gfx_def.h>
#include <bgfx/c99/bgfx.h>
#include <utils.hpp>

namespace pg::gfx
{

////////////////////////////////////////////////
class ShaderProgram : public pg::utils::NonCopyable
{
public:
    ShaderProgram();
    ShaderProgram(std::string const& vertexShaderFile, std::string const& fragmentShaderFile);
    ShaderProgram(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader);

    ShaderProgram(ShaderProgram&& rhs);
    ShaderProgram& operator=(ShaderProgram&& rhs);

    ~ShaderProgram();

    bgfx_program_handle_t GetHandle() const;


private:
    std::shared_ptr<bgfx_shader_handle_t> m_VertexShaderHandle;
    std::shared_ptr<bgfx_shader_handle_t> m_FragmentShaderHandle;

    bgfx_program_handle_t m_ProgramHandle;
};

using SharedShaderProgram = std::shared_ptr<ShaderProgram>;


////////////////////////////////////////////////
class Texture : public pg::utils::NonCopyable
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

class VertexLayout : public utils::NonCopyable
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

class VertexBuffer : public utils::NonCopyable
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

}

