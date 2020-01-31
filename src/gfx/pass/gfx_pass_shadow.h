#pragma once

#include <class_features/class_features.hpp>
#include <gfx/gfx_objects.h>
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

private:
    bgfx_frame_buffer_handle_t m_Framebuffer;

    SharedTexture m_ShadowMap;
};

}
