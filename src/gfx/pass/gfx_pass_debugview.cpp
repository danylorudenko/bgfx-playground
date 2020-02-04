#include <gfx/pass/gfx_pass_debugview.h>

#include <bgfx/c99/bgfx.h>
#include <glm/gtc/type_ptr.hpp>

#include <gfx/gfx_settings.h>
#include <scene/scene.h>

namespace pg::gfx
{

PassDebugView::PassDebugView(PassId passId)
    : PassBase{ passId }
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

    // hmm, how should I draw debug rect
}

}

