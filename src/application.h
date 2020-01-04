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
    Application(char const* title, WindowClass::WinProcHandler procHandler);

    inline Window*         GetMainWindow() const { return m_MainWindow.get(); }

private:
    std::unique_ptr<Window>         m_MainWindow;
};

} // namespace pg
