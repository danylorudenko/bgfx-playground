#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_def.h>

namespace pg::bgfx_helpers
{

void uniqueShaderDestroyProxy(bgfx_shader_handle_t const* handle);
std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(std::string const& fileName);
std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(bgfx_memory_t const* mem);


std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(std::string const& fileName);
std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(bgfx_memory_t const* mem);

} // namespace pg::bgfx_helpers

