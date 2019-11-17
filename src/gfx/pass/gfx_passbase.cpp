#include <gfx/pass/gfx_passbase.h>

#include <cassert>
#include <algorithm>
#include <limits>

#include <gfx/gfx_settings.h>
#include <memory_helpers.h>

namespace pg::gfx
{

auto DestroyFramebuffer = [](bgfx_frame_buffer_handle_t& handle) { if(BGFX_HANDLE_IS_VALID(handle)) bgfx_destroy_frame_buffer(handle); };

PassBase::PassBase(PassSchedule scheduleId)
    : m_Framebuffer{ DestroyFramebuffer, bgfx_frame_buffer_handle_t{ BGFX_INVALID_HANDLE } }
    , m_ScheduleId{ scheduleId }
    , m_Immutable{ false }
{}

PassBase::PassBase(PassBase&& rhs) = default;

PassBase& PassBase::operator=(PassBase&& rhs) = default;

PassBase::~PassBase() = default;

PassSchedule PassBase::ScheduleId() const
{
    return m_ScheduleId;
}

void PassBase::Begin()
{
    bgfx_frame_buffer_handle_t const framebuffer = m_Framebuffer.Ref();

    if (BGFX_HANDLE_IS_VALID(framebuffer))
    {
        bgfx_set_view_frame_buffer(m_ScheduleId, framebuffer);
    }
}

void PassBase::Render()
{

}

void PassBase::AddScreenSpaceAttachment()
{
    assert(!m_Immutable && "Attempt to modify immutable pass.");

    Attachment attachment;

    attachment.texture = TextureRef{ 
        gfx::settings::g_MainResolutionX,
        gfx::settings::g_MainResolutionY,
        gfx::settings::g_BackbufferFormat,
        TextureUsage::RenderTarget 
    };

    bgfx_attachment_init(&attachment.attachment, attachment.texture.GetHandle(), BGFX_ACCESS_READWRITE, 0, 0, BGFX_RESOLVE_NONE);

    m_Attachments.emplace_back(attachment);
}

void PassBase::Bake()
{
    std::size_t const attachmentsCount = m_Attachments.size();
    if (attachmentsCount)
    {
        // if framebuffer is backbuffer, no other attachments are allowed
        m_Immutable = true;
        return;
    }

    std::vector<bgfx_attachment_t> bgfxAttachmentVector{ attachmentsCount };
    for (std::size_t i = 0; i < attachmentsCount; i++) {
        bgfxAttachmentVector[i] = m_Attachments[i].attachment;
    }

    assert(attachmentsCount <= 8 && "Can't have more than 8 attachments in framebuffer.");
    m_Framebuffer.Ref() = bgfx_create_frame_buffer_from_attachment(
        static_cast<std::uint8_t>(attachmentsCount), 
        bgfxAttachmentVector.data(), 
        false
    );

    m_Immutable = true;
}

} // namespace pg::gfx
