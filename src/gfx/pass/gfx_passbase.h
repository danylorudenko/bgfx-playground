#pragma once

#include <vector>

#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_objects.h>
#include <gfx/pass/gfx_pass_schedule.h>
#include <utils.hpp>

namespace pg
{
class Scene;
} // namespace pg

namespace pg::gfx
{

class PassBase
{
public:
    PassBase(PassId scheduleId);
    PassBase(PassBase&& rhs);
    PassBase& operator=(PassBase&& rhs);

    virtual ~PassBase();

    PassBase(PassBase const&) = delete;
    PassBase& operator=(PassBase const&) = delete;

    PassId GetPassId() const;

    virtual void Render(Scene* scene) = 0;

    virtual void AddScreenSpaceAttachment();
    virtual void Bake();


protected:
    struct Attachment
    {
        TextureRef texture;
        bgfx_attachment_t attachment;
    };

    std::vector<Attachment> m_Attachments;
    utils::AutoDestroyed<bgfx_frame_buffer_handle_t> m_Framebuffer;

    PassId m_PassId;
    bool m_Immutable;
};

}

