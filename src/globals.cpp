#include <globals.h>

namespace pg
{

Application::Application(char const* title, std::uint32_t width, std::uint32_t height, WindowClass::WinProcHandler procHandler)
    : m_MainWindow{ std::make_unique<Window>(::GetModuleHandleA(NULL), title, width, height, "MyWindowClass", procHandler, this) }
    , m_MainRenderer{ std::make_unique<gfx::Renderer>() }
    , m_MainScene{ std::make_unique<Scene>() }
{

}

} // namespace pg

