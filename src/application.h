#pragma once

#include <memory>

#include <gfx/gfx_renderer.h>
#include <scene/scene.h>
#include <utils.hpp>
#include <windows/window.h>


namespace pg
{

class Application 
    : public utils::NonCopyable
    , public utils::StaticSingleton<Application>
{
public:
    Application(char const* title, std::uint32_t width, std::uint32_t height, WindowClass::WinProcHandler procHandler);

    inline Window*         GetMainWindow() const { return m_MainWindow.get(); }

private:
    std::unique_ptr<Window>         m_MainWindow;
};

} // namespace pg
