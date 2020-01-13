#pragma once

#include <gfx/gfx_objects.h>

namespace pg
{

struct RenderableComponent
{
    RenderableComponent(gfx::SharedShaderProgram const& shader, gfx::SharedVertexBuffer const& vertexBuffer)
        : m_ShaderProgram{ shader }
        , m_VertexBuffer{ vertexBuffer }
    {}

    gfx::SharedShaderProgram        m_ShaderProgram;
    gfx::SharedVertexBuffer         m_VertexBuffer;

    gfx::UniformRawData             m_RawData;
    gfx::UniformTextureData         m_TextureData;
};

} // namespace pg
