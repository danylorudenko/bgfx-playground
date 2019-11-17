#pragma once

#include <vector>

#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_objects.h>
#include <gfx/pass/gfx_pass_schedule.h>
#include <utils.hpp>

namespace pg::gfx
{

class PassBase
{
public:
    PassBase(PassSchedule scheduleId);
    PassBase(PassBase&& rhs);
    PassBase& operator=(PassBase&& rhs);

    virtual ~PassBase();

    PassBase(PassBase const&) = delete;
    PassBase& operator=(PassBase const&) = delete;

    PassSchedule ScheduleId() const;

    virtual void Begin() = 0;
    virtual void Render() = 0;

    virtual void AddScreenSpaceAttachment();
    virtual void Bake();


private:
    struct Attachment
    {
        TextureRef texture;
        bgfx_attachment_t attachment;
    };

    std::vector<Attachment> m_Attachments;
    utils::AutoDestroyed<bgfx_frame_buffer_handle_t> m_Framebuffer;

    PassSchedule m_ScheduleId;
    bool m_Immutable;
};

}

