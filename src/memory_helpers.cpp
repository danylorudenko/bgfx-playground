#include <memory_helpers.h>

namespace pg::memory_helpers
{

std::shared_ptr<bgfx_vertex_buffer_handle_t> makeSharedVertexBuffer(bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout, std::uint16_t flags)
{
    bgfx_vertex_buffer_handle_t vertexBuffer = bgfx_create_vertex_buffer(mem, layout, flags);
    return std::shared_ptr<bgfx_vertex_buffer_handle_t>(new bgfx_vertex_buffer_handle_t{ vertexBuffer },
        [](bgfx_vertex_buffer_handle_t* vertexBufferPtr)
        {
            bgfx_destroy_vertex_buffer(*vertexBufferPtr);
        });
}

} // namespace pg::memory_helpers

