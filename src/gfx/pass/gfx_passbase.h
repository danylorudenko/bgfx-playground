#pragma once

#include <vector>

#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_objects.h>
#include <gfx/pass/gfx_pass_schedule.h>
#include <class_features/class_features.hpp>

namespace pg
{
class Scene;
} // namespace pg

namespace pg::gfx
{

class PassBase : public class_features::NonCopyable
{
public:
    PassBase(PassId scheduleId);
    PassBase(PassBase&& rhs);
    PassBase& operator=(PassBase&& rhs);

    virtual ~PassBase();

    PassId GetPassId() const;

    virtual void Render(Scene* scene) = 0;

protected:
    PassId m_PassId;
    bool m_Immutable;
};

}

