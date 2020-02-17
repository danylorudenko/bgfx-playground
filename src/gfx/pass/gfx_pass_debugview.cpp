#include <gfx/pass/gfx_pass_debugview.h>

#include <bgfx/c99/bgfx.h>
#include <glm/gtc/type_ptr.hpp>

#include <gfx/gfx_resourcemap.h>
#include <gfx/gfx_settings.h>
#include <scene/scene.h>

namespace pg::gfx
{

float g_QuadVertices[] = 
{
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f
};

PassDebugView::PassDebugView(PassId passId)
    : PassBase{ passId }
    , m_Enabled{ false }
    , m_RectDrawProgram{ std::make_shared<ShaderProgram>("shaders\\vs_draw_fullscreen_quad.bin", "shaders\\fs_draw_fullscreen_quad.bin") }
    , m_DebugTextureUniformProxy{ std::make_shared<UniformProxy>("u_DebugTexture", BGFX_UNIFORM_TYPE_SAMPLER) }
{
    m_QuadVertexLayout = std::make_shared<VertexLayout>();
    m_QuadVertexLayout->AddAtribute(BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_QuadVertexLayout->AddAtribute(BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false);
    m_QuadVertexLayout->Bake();

    m_QuadVertexBuffer = std::make_shared<VertexBuffer>(m_QuadVertexLayout, 6, g_QuadVertices, (std::uint32_t)sizeof(g_QuadVertices));
}

PassDebugView::PassDebugView(PassDebugView&& rhs) = default;

PassDebugView& PassDebugView::operator=(PassDebugView&& rhs) = default;

PassDebugView::~PassDebugView() = default;

void PassDebugView::SetEnabled(bool value)
{
    m_Enabled = value;
}

bool PassDebugView::GetEnabled() const
{
    return m_Enabled;
}

void PassDebugView::Render(Scene* scene)
{
    if (!m_Enabled)
        return;

    PassBase::Render(scene);

    ////////////////////////////////

    PassId const passId = m_PassId;
    bgfx_set_view_name(passId, "PassDebugView");

    ////////////////////////////////

    std::uint16_t C_DEBUG_RECT_EXTENT = 200;

    std::uint16_t C_DEBUG_RECT[4] = { 
        static_cast<std::uint16_t>(gfx::settings::g_MainResolutionX - C_DEBUG_RECT_EXTENT),
        static_cast<std::uint16_t>(gfx::settings::g_MainResolutionY - C_DEBUG_RECT_EXTENT),
        C_DEBUG_RECT_EXTENT,
        C_DEBUG_RECT_EXTENT,
    };

    bgfx_set_view_rect(passId, C_DEBUG_RECT[0], C_DEBUG_RECT[1], C_DEBUG_RECT[2], C_DEBUG_RECT[3]);
    bgfx_set_view_scissor(passId, C_DEBUG_RECT[0], C_DEBUG_RECT[1], C_DEBUG_RECT[2], C_DEBUG_RECT[3]);

    bgfx_set_state(BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_WRITE_MASK | BGFX_STATE_CULL_CW, 0);
    bgfx_set_vertex_buffer(0, m_QuadVertexBuffer->GetHandle(), 0, m_QuadVertexBuffer->GetVertexCount());

    SharedTexture& shadowMap = g_TextureMap[TextureName::kShadowMap];
    m_DebugTextureUniformProxy->SetTexture(shadowMap.get(), BGFX_SAMPLER_NONE);

    bgfx_submit(passId, m_RectDrawProgram->GetHandle(), 0, false);
}

}

