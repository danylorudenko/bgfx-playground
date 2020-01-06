#pragma once

#include <cassert>
#include <utility>

namespace pg::class_features
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


template<typename T>
class StaticSingleton
{
public:
    static T* GetInstance() { return s_Instance; }

    template<typename... TArgs>
    static T* CreateInstance(TArgs&&... args)
    {
        assert(s_Instance == nullptr && "Attempt to create singleton instance second time.");
        return s_Instance = new T{ std::forward<TArgs>(args)... };
    }

    static void DestroyInstance()
    {
        assert(s_Instance != nullptr && "Attemp to destroy singleton that has not been instantiated.");
        delete s_Instance;
        s_Instance = nullptr;
    }

private:
    static T* s_Instance;
};

template<typename T> T* StaticSingleton<T>::s_Instance = nullptr;

} // namespace pg::utils

