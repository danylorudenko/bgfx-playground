#pragma once

#include <gfx/gfx_objects.h>

namespace pg
{

struct RenderableComponent
{
    RenderableComponent(gfx::ShaderRef const& shader, gfx::VertexBufferRef const& vertexBuffer)
        : m_ShaderProgram{ shader }
        , m_VertexBuffer{ vertexBuffer }
    {}

    gfx::ShaderRef                                  m_ShaderProgram;
    gfx::VertexBufferRef                            m_VertexBuffer;
};

} // namespace pg
