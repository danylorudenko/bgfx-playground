#include <gfx/gfx_renderer.h>

#include <cassert>

#include <bgfx/c99/bgfx.h>
#include <globals.h>

#include <gfx/pass/gfx_pass_forward.h>

namespace pg::gfx
{

Renderer::Renderer()
{
    m_PassQueue.emplace_back(std::make_unique<PassForward>(PassId::kForward));
}

Renderer::Renderer(Renderer&&) = default;

Renderer& Renderer::operator=(Renderer&& rhs) = default;

Renderer::~Renderer() = default;

void Renderer::Update()
{
    PassId scheduleGuard = PassId::kNone;
    for (auto& pass : m_PassQueue)
    {
        PassId passId = pass->GetPassId();
        assert((int)passId - 1 == (int)scheduleGuard && "Pass ordering schedule is violated.");

        pass->Render(&g_MainScene);
    }

    bgfx_frame(false);
}


}

