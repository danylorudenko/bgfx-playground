#include <struct_helpers.h>
#include <gfx/gfx_settings.h>

namespace pg::struct_helpers
{

bgfx_init_t bgfxInitDefault()
{
    // ==================
    gfx::settings::g_MainResolutionX = 800;
    gfx::settings::g_MainResolutionY = 600;
    gfx::settings::g_BackbufferFormat = BGFX_TEXTURE_FORMAT_RGBA8;
    // ==================
    
    bgfx_init_t initStruct;
    initStruct.type = BGFX_RENDERER_TYPE_COUNT;
    initStruct.vendorId = BGFX_PCI_ID_NONE;
    initStruct.deviceId = 0;
    initStruct.debug = true;
    initStruct.profile = true;

    initStruct.platformData.ndt = nullptr;
    initStruct.platformData.nwh = g_MainWindow->NativeHandle();
    initStruct.platformData.context = nullptr;
    initStruct.platformData.backBuffer = nullptr;
    initStruct.platformData.backBufferDS = nullptr;

    initStruct.resolution.width = gfx::settings::g_MainResolutionX;
    initStruct.resolution.height = gfx::settings::g_MainResolutionY;
    initStruct.resolution.reset = BGFX_RESET_VSYNC;
    initStruct.resolution.format = gfx::settings::g_BackbufferFormat;
    initStruct.resolution.numBackBuffers = 2;

    initStruct.limits.maxEncoders = 4;
    initStruct.limits.transientVbSize = 1024 * 1024 * 16;
    initStruct.limits.transientIbSize = 1024 * 1024 * 8;

    initStruct.callback = nullptr;
    initStruct.allocator = nullptr;

    return initStruct;
}

} // namespace pg::struct_helpers
