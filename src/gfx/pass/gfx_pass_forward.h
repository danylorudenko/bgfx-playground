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

    virtual void Begin() override;
    virtual void Render() override;
};

} // namespace pg::gfx

