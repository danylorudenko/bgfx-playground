#pragma once

#include <cstdint>
#include <bgfx/c99/bgfx.h>

namespace pg::gfx
{

enum class PassIdKey
{
    kNone = -1,
    kShadow = 0,
    kForward,

    // this should be the last one, so it can easily draw to swapchain RT
    kDebugView
};

class PassId
{
public:
    PassId(PassIdKey key = PassIdKey::kNone) : m_Key{ key } {}

    operator bgfx_view_id_t() const
    {
        return static_cast<bgfx_view_id_t>(m_Key);
    }

    operator std::int32_t() const
    {
        return static_cast<std::int32_t>(m_Key);
    }

    bool operator<(PassId rhs) const
    {
        return static_cast<std::int32_t>(m_Key) < static_cast<std::int32_t>(rhs.m_Key);
    }

    bool operator>(PassId rhs) const
    {
        return static_cast<std::int32_t>(m_Key) > static_cast<std::int32_t>(rhs.m_Key);
    }

    bool operator==(PassId rhs) const
    {
        return static_cast<std::int32_t>(m_Key) == static_cast<std::int32_t>(rhs.m_Key);
    }

    bool operator!=(PassId rhs) const
    {
        return static_cast<std::int32_t>(m_Key) != static_cast<std::int32_t>(rhs.m_Key);
    }
private:
    PassIdKey m_Key;
};

} // namespace pg::gfx

