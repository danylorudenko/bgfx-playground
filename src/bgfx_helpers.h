#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_def.h>

namespace pg::bgfx_helpers
{

std::shared_ptr<bgfx_vertex_buffer_handle_t> makeSharedVertexBuffer(bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout, std::uint16_t flags = BGFX_BUFFER_NONE);


void uniqueShaderDestroyProxy(bgfx_shader_handle_t const* handle);
std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(std::string const& fileName);
std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(bgfx_memory_t const* mem);



std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(std::string const& fileName);
std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(bgfx_memory_t const* mem);


// does not own argument shaders
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_shader_handle_t vsHandle, bgfx_shader_handle_t fsHandle);


// creates shaders and destroys them on program destruction
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_memory_t const* vsMem, bgfx_memory_t const* fsMem);


// reads files, creates shaders, prgram, destroys shaders on program destruction
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(std::string const& vsFileName, std::string const& fsFileName);



std::shared_ptr<bgfx_texture_handle_t> makeShared2DTexture(std::string const& fileName, int componentsCount = 3, int* widthOut = nullptr, int* heightOut = nullptr);


// makes empty texture
std::shared_ptr<bgfx_texture_handle_t> makeShared2DTexture(std::uint32_t width, std::uint32_t height, bgfx_texture_format format, gfx::TextureUsage usage);



std::shared_ptr<bgfx_uniform_handle_t> makeSharedUniform(std::string const& name, bgfx_uniform_type type, std::uint32_t arrayElementsCount = 1);


} // namespace pg::bgfx_helpers

