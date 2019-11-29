#pragma once

#include <bgfx/c99/bgfx.h>
#include <memory>
#include <string>
#include <cstdint>
#include <gfx/gfx_def.h>

namespace pg::gfx
{

////////////////////////////////////////////////
class ShaderRef
{
public:
    ShaderRef(bgfx_program_handle_t handle = BGFX_INVALID_HANDLE);
    ShaderRef(std::string const& vertexShaderFile, std::string const& fragmentShaderFile);
    ShaderRef(std::shared_ptr<bgfx_shader_handle_t> const& vertexShader, std::shared_ptr<bgfx_shader_handle_t> const& fragmentShader);
    ~ShaderRef() = default;

    bgfx_program_handle_t GetHandle() const;


private:
    std::shared_ptr<bgfx_shader_handle_t> m_VertexShaderHandle;
    std::shared_ptr<bgfx_shader_handle_t> m_FragmentShaderHandle;

    std::shared_ptr<bgfx_program_handle_t> m_ProgramHandle;
};


////////////////////////////////////////////////
class TextureRef
{
public:
    TextureRef(std::uint32_t width, std::uint32_t height, bgfx_texture_format format, TextureUsage usage);
    TextureRef(std::string const& textureFile);
    ~TextureRef() = default;

    bgfx_texture_handle_t GetHandle() const;

private:
    std::shared_ptr<bgfx_texture_handle_t> m_TextureHandle;
    std::uint32_t m_Width;
    std::uint32_t m_Height;
};


////////////////////////////////////////////////
class VertexBufferRef
{
public:
    VertexBufferRef(std::uint32_t vertexCount, bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout);
    ~VertexBufferRef();

    bgfx_vertex_buffer_handle_t GetHandle() const;
    std::uint32_t GetVertexCount() const;

private:
    std::shared_ptr<bgfx_vertex_buffer_handle_t> m_VertexBufferHandle;
    std::uint32_t m_VertexCount;
};

}

