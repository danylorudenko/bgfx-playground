#pragma once

#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class PassForward : public PassBase
{
public:
    PassForward(PassId scheduleId);
    PassForward(PassForward&& rhs);
    PassForward& operator=(PassForward&& rhs);

    virtual ~PassForward();

    virtual void Render(Scene* scene) override;

private:
    UniformProxy m_RawDataProxy;
    UniformProxy m_TextureProxy;
};

} // namespace pg::gfx

