#pragma once

#include <gfx/gfx_objects.h>

namespace pg
{

struct RenderableComponent
{
    gfx::ShaderRef                                  m_ShaderProgram;
    gfx::VertexBufferRef                            m_VertexBuffer;
    std::shared_ptr<bgfx_uniform_handle_t>          m_ModelTransformUniform;
    std::shared_ptr<bgfx_uniform_handle_t>          m_ColorTransformUniform;
};

} // namespace pg
