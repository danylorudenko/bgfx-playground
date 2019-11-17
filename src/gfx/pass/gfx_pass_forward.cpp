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

void PassForward::Begin()
{
    PassBase::Begin();
}

void PassForward::Render()
{
    PassBase::Render();
}

}

