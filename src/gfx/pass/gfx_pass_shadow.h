#pragma once

#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class PassShadow : public PassBase
{
public:
    PassShadow(PassId scheduleId);
    PassShadow(PassShadow&& rhs);
    PassShadow& operator=(PassShadow&& rhs);

    virtual ~PassShadow();

    virtual void Render(Scene* scene) override;
};

}
