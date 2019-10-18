#include <cassert>
#include <Windows.h>

#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

#include <globals.h>
#include <Windows/Window.h>
#include <entity.h>
#include <struct_helpers.h>

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
int main()
{
    HINSTANCE hInstance = GetModuleHandleA(NULL);
    g_MainWindow = Window{ hInstance, "MyWindow", 800, 600, "MyWindowClass", &MyProcHandler, nullptr };

    bgfx_init_t initStruct = bgfx_init_t_default();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");


    //bgfx_create_s


    mainLoop();

    bgfx_shutdown();

    return 0;
}

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
