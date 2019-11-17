#pragma once

#include <utility>

namespace pg::utils
{

class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
    ~NonCopyable() = default;

    NonCopyable(NonCopyable const&) = delete;
    NonCopyable& operator=(NonCopyable const&) = delete;
};


template<typename T>
class AutoDestroyed
{
public:
    using DestroyDelegate = void(*)(T&);
    
    template<typename TDestructor, typename... TArgs>
    AutoDestroyed(TDestructor destroyDelegate, TArgs&&... args)
        : m_Data{ std::forward<TArgs>(args)... }
        , m_DestroyDelegate{ destroyDelegate }
    {}

    T& Ref() { return m_Data; }
    T const& Ref() const { return m_Data; }

    ~AutoDestroyed()
    {
        m_DestroyDelegate(m_Data);
    }

private:
    T m_Data;
    DestroyDelegate m_DestroyDelegate;
};

} // namespace pg::utils

