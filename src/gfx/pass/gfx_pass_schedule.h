#pragma once

namespace pg::gfx
{

enum PassId
{
    kNone = -1,
    kShadow = 0,
    kForward,

    // this should be the last one, so it can easily draw to swapchain RT
    kDebugView
};

} // namespace pg::gfx

