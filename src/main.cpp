#include <cassert>
#include <Windows.h>

#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

#include <globals.h>
#include <windows/window.h>
#include <entity.h>
#include <struct_helpers.h>
#include <io_helpers.h>

namespace pg
{
LRESULT MyProcHandler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProcA(handle, message, wparam, lparam);
}

void mainLoop();
void mainUpdate();

Entity testEntity;

} // namespace pg


int main()
{
    HINSTANCE hInstance = GetModuleHandleA(NULL);
    pg::g_MainWindow = pg::Window{ hInstance, "MyWindow", 800, 600, "MyWindowClass", &pg::MyProcHandler, nullptr };

    bgfx_init_t initStruct = pg::bgfx_init_t_default();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");


    pg::FileReader fileReader{ "shaders\\vs_triangle.bin" };
    bgfx_memory_t const* shaderData = fileReader.ReadToBgfx();
    bgfx_shader_handle_t shaderHadle = bgfx_create_shader(shaderData);


    pg::mainLoop();

    bgfx_shutdown();

    return 0;
}

namespace pg
{
void mainLoop()
{
    MSG message{};
    message.message = WM_NULL;

    while (message.message != WM_QUIT) {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else {
            mainUpdate();
            bgfx_frame(false);
        }
    }
}

void mainUpdate()
{

}

} // namespace pg

