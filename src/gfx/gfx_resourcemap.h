#pragma once

#include <unordered_map>

#include <gfx/gfx_objects.h>

namespace pg::gfx
{

enum TextureId
{
    kNone = -1
};

//enum BufferId
//{
//    kNone = -1
//};

using TextureMap = std::unordered_map<TextureId, TextureRef>;
extern TextureMap g_TextureMap;

//using BufferMap = 



} // namespace pg::gfx

