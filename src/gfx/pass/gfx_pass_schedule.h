#pragma once

#include <bgfx/c99/bgfx.h>

namespace pg::gfx
{

enum class PassId
{
    kNone = -1,
    kShadow = 0,
    kForward,

    // this should be the last one, so it can easily draw to swapchain RT
    kDebugView
};

} // namespace pg::gfx

