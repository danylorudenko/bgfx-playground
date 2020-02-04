#pragma once

#include <unordered_map>

#include <gfx/gfx_objects.h>

namespace pg::gfx
{

enum TextureName
{
    kNone = -1,
    kSwapchain = 0,
    kShadowMap
};

using TextureMap = std::unordered_map<TextureName, SharedTexture>;
extern TextureMap g_TextureMap;


} // namespace pg::gfx

