#include <Windows.h>

#include <Windows/Window.h>
#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

Window g_MainWindow;

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

int main()
{
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    g_MainWindow = Window{ hInstance, "MyWindow", 800, 600, "MyWindowClass", &MyProcHandler, nullptr };

    bgfx_resolution_t resolution;
    resolution.width = 800;
    resolution.height = 600;
    resolution.format = BGFX_TEXTURE_FORMAT_RGBA8;
    resolution.numBackBuffers = 2;

    bgfx_init_t initStruct;
    initStruct.resolution = resolution;
    initStruct.debug = true;
    initStruct.profile = true;
    initStruct.platformData.nwh = g_MainWindow.NativeHandle();



    bgfx_init(&initStruct);

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
