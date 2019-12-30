#pragma once

#include <gfx/gfx_objects.h>

namespace pg
{

struct RenderableComponent
{
    RenderableComponent(gfx::ShaderProgram const& shader, gfx::VertexBufferRef const& vertexBuffer)
        : m_ShaderProgram{ shader }
        , m_VertexBuffer{ vertexBuffer }
    {}

    gfx::ShaderProgram                                  m_ShaderProgram;
    gfx::VertexBufferRef                            m_VertexBuffer;
};

} // namespace pg
