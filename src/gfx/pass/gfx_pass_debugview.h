#pragma once

#include <class_features/class_features.hpp>
#include <gfx/gfx_objects.h>
#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class PassDebugView : public PassBase
{
public:
    PassDebugView(PassId scheduleId);
    PassDebugView(PassDebugView&& rhs);
    PassDebugView& operator=(PassDebugView&& rhs);

    virtual ~PassDebugView();

    virtual void Render(Scene* scene) override;

private:
    //SharedFramebuffer m_Framebuffer;
    //
    //SharedTexture m_ShadowMap;
    //SharedShaderProgram m_ShadowGenProgram;
};

}
