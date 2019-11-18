#include <gfx/gfx_renderer.h>

#include <cassert>

#include <bgfx/c99/bgfx.h>
#include <globals.h>

namespace pg::gfx
{

Renderer::Renderer() = default;

Renderer::Renderer(Renderer&&) = default;

Renderer& Renderer::operator=(Renderer&& rhs) = default;

Renderer::~Renderer() = default;

void Renderer::Update()
{
    PassSchedule scheduleGuard = PassSchedule::kNone;
    for (auto& pass : m_PassQueue)
    {
        PassSchedule passId = pass->ScheduleId();
        assert((int)passId > (int)scheduleGuard && "Pass ordering schedule is violated.");

        pass->Begin(&g_MainScene);
        pass->Render(&g_MainScene);
    }

    bgfx_frame(false);
}


}

