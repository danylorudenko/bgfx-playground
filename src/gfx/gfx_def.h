#pragma once

namespace pg::gfx
{

enum class TextureUsage
{
    None,
    Sampled,
    RenderTarget,
    DepthReadWrite,
    DepthRead,
    DepthWrite
};

}

