#pragma once

#include <vector>

#include <bgfx/c99/bgfx.h>
#include <gfx/gfx_objects.h>

namespace pg::gfx
{

class Pass
{
public:
    Pass();
    Pass(Pass&& rhs);
    Pass& operator=(Pass&& rhs);
    ~Pass();

    Pass(Pass const&) = delete;
    Pass& operator=(Pass const&) = delete;

    void Begin();
    void End();

    void AddScreenSpaceAttachment();


private:
    struct Attachment
    {
        TextureRef texture;
        bgfx_attachment_t attachment;
    };

    std::vector<bgfx_attachment_t> m_Attachments;
};

}

