#pragma once

#include <memory>
#include <cstdint>
#include <bgfx/c99/bgfx.h>

namespace pg::memory_helpers
{

std::shared_ptr<bgfx_vertex_buffer_handle_t> makeSharedVertexBuffer(bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout, std::uint16_t flags = BGFX_BUFFER_NONE);

} // namespace pg::memory_helpers

