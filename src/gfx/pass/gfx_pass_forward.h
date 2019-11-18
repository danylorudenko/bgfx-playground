#pragma once

#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class PassForward : public PassBase
{
public:
    PassForward(PassSchedule scheduleId);
    PassForward(PassForward&& rhs);
    PassForward& operator=(PassForward&& rhs);

    virtual ~PassForward();

    virtual void Begin(Scene* scene) override;
    virtual void Render(Scene* scene) override;
};

} // namespace pg::gfx

