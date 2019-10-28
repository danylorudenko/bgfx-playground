#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <bgfx/c99/bgfx.h>

namespace pg::memory_helpers
{

std::shared_ptr<bgfx_vertex_buffer_handle_t> makeSharedVertexBuffer(bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout, std::uint16_t flags = BGFX_BUFFER_NONE);



std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(bgfx_memory_t const* mem);


// does not own argument shaders
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_shader_handle_t vsHandle, bgfx_shader_handle_t fsHandle);


// creates shaders and destroys them on program destruction
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_memory_t const* vsMem, bgfx_memory_t const* fsMem);


// reads files, creates shaders, prgram, destroys shaders on program destruction
std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(std::string const& vsFileName, std::string const& fsFileName);



std::shared_ptr<bgfx_texture_handle_t> makeShared2DTexture(std::string const& fileName, int componentsCount = 3);


} // namespace pg::memory_helpers

