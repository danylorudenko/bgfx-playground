#include <struct_helpers.h>

namespace pg
{

bgfx_init_t bgfx_init_t_default()
{
    bgfx_init_t initStruct;
    initStruct.type = BGFX_RENDERER_TYPE_DIRECT3D11;
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

    return initStruct;
}

} // namespace pg
