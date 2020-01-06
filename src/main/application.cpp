#include <application.h>

#include <gfx/gfx_settings.h>

namespace pg
{

Application::Application(char const* title, WindowClass::WinProcHandler procHandler)
    : m_MainWindow{ 
        std::make_unique<Window>(
            ::GetModuleHandleA(NULL), 
            title, 
            gfx::settings::g_MainResolutionX, 
            gfx::settings::g_MainResolutionY, 
            "MyWindowClass", 
            procHandler, 
            this) 
    }
{

}

} // namespace pg

