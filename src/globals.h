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
    inline Scene*          GetMainScene() const { return m_MainScene.get(); }
    inline gfx::Renderer*  GetMainRenderer() const { return m_MainRenderer.get(); }

private:
    std::unique_ptr<Window>         m_MainWindow;
    std::unique_ptr<gfx::Renderer>  m_MainRenderer;
    std::unique_ptr<Scene>          m_MainScene;
};

} // namespace pg
