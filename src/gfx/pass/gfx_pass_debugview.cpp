#include <gfx/pass/gfx_pass_debugview.h>

#include <bgfx/c99/bgfx.h>
#include <glm/gtc/type_ptr.hpp>

#include <gfx/gfx_resourcemap.h>
#include <gfx/gfx_settings.h>
#include <scene/scene.h>

namespace pg::gfx
{

PassDebugView::PassDebugView(PassId passId)
    : PassBase{ passId }
    , m_RectDrawProgram{ std::make_shared<ShaderProgram>("shaders\\vs_draw_fullscreen_quad.bin", "shaders\\fs_draw_fullscreen_quad.bin") }
    , m_DebugTextureUniformProxy{ std::make_shared<UniformProxy>("u_DebugTexture", BGFX_UNIFORM_TYPE_SAMPLER) }
{
}

PassDebugView::PassDebugView(PassDebugView&& rhs) = default;

PassDebugView& PassDebugView::operator=(PassDebugView&& rhs) = default;

PassDebugView::~PassDebugView() = default;

void PassDebugView::Render(Scene* scene)
{
    PassBase::Render(scene);

    ////////////////////////////////

    PassId const passId = m_PassId;
    bgfx_set_view_name(passId, "PassDebugView");

    ////////////////////////////////

    std::uint16_t C_DEBUG_RECT[4] = { gfx::settings::g_MainResolutionX - 100, gfx::settings::g_MainResolutionY - 100, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY };
    
    bgfx_set_view_rect(passId, C_DEBUG_RECT[0], C_DEBUG_RECT[1], C_DEBUG_RECT[2], C_DEBUG_RECT[3]);
    bgfx_set_view_scissor(passId, C_DEBUG_RECT[0], C_DEBUG_RECT[1], C_DEBUG_RECT[2], C_DEBUG_RECT[3]);

    bgfx_set_state(BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_WRITE_MASK | BGFX_STATE_CULL_CCW);

    SharedTexture& shadowMap = g_TextureMap[TextureName::kShadowMap];
    m_DebugTextureUniformProxy->SetTexture(shadowMap.get(), BGFX_SAMPLER_NONE);

    bgfx_submit(passId, m_RectDrawProgram->GetHandle(), 0, false);

    // hmm, how should I draw debug rect
}

}

