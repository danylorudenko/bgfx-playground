#pragma once

#include <memory>

#include <class_features/class_features.hpp>
#include <gfx/gfx_renderer.h>
#include <scene/scene.h>
#include <system/window.h>


namespace pg
{

class Application 
    : public class_features::NonCopyable
    , public class_features::StaticSingleton<Application>
{
public:
    Application(char const* title, WindowClass::WinProcHandler procHandler);

    inline Window*         GetMainWindow() const { return m_MainWindow.get(); }

private:
    std::unique_ptr<Window>         m_MainWindow;
};

} // namespace pg
