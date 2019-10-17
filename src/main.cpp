#include <cassert>
#include <Windows.h>

#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

#include <Windows/Window.h>
#include <entity.h>

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
    Entity testEntity{};
    
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    g_MainWindow = Window{ hInstance, "MyWindow", 800, 600, "MyWindowClass", &MyProcHandler, nullptr };

    bgfx_init_t initStruct;
    initStruct.type = BGFX_RENDERER_TYPE_COUNT;
    initStruct.vendorId = BGFX_PCI_ID_NONE;
    initStruct.deviceId = 0;
    initStruct.debug = true;
    initStruct.profile = true;

    initStruct.platformData.ndt = nullptr;
    initStruct.platformData.nwh = g_MainWindow.NativeHandle();
    initStruct.platformData.context = nullptr;
    initStruct.platformData.backBuffer = nullptr;
    initStruct.platformData.backBufferDS = nullptr;

    initStruct.resolution.width = 800;
    initStruct.resolution.height = 600;
    initStruct.resolution.format = BGFX_TEXTURE_FORMAT_RGBA8;
    initStruct.resolution.numBackBuffers = 2;

    initStruct.limits.maxEncoders = 4;
    initStruct.limits.transientVbSize = 1024 * 1024 * 16;
    initStruct.limits.transientIbSize = 1024 * 1024 * 8;

    initStruct.callback = nullptr;
    initStruct.allocator = nullptr;

    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");

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
