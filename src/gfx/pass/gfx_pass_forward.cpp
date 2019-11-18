#include <gfx/pass/gfx_pass_forward.h>

#include <utility>

namespace pg::gfx
{

PassForward::PassForward(PassSchedule scheduleId)
    : PassBase{ scheduleId }
{}

PassForward::PassForward(PassForward&& rhs)
    : PassBase{ std::move(rhs) }
{
}

PassForward& PassForward::operator=(PassForward&& rhs)
{
    PassBase::operator=(std::move(rhs));

    return *this;
}

PassForward::~PassForward()
{
}

void PassForward::Begin(Scene* scene)
{
    PassBase::Begin(scene);

    ////////////////////////////////

    bgfx_frame_buffer_handle_t const framebuffer = m_Framebuffer.Ref();

    if (BGFX_HANDLE_IS_VALID(framebuffer))
    {
        bgfx_set_view_frame_buffer(m_ScheduleId, framebuffer);
    }
}

void PassForward::Render(Scene* scene)
{
    PassBase::Render(scene);
}

}

