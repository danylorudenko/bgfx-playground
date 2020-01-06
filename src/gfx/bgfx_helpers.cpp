#include <gfx/bgfx_helpers.h>

#include <cassert>

#include <stb/stb_image.h>

#include <io/io_helpers.h>
#include <gfx/gfx_settings.h>
#include <main/application.h>

namespace pg::bgfx_helpers
{

bgfx_init_t bgfxInitDefault(bool debug)
{
    bgfx_init_t initStruct;
    initStruct.type = BGFX_RENDERER_TYPE_COUNT;
    initStruct.vendorId = BGFX_PCI_ID_NONE;
    initStruct.deviceId = 0;
    initStruct.debug = debug;
    initStruct.profile = true;

    initStruct.platformData.ndt = nullptr;
    initStruct.platformData.nwh = Application::GetInstance()->GetMainWindow()->NativeHandle();
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

void uniqueShaderDestroyProxy(bgfx_shader_handle_t const* handle) { bgfx_destroy_shader(*handle); }

std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(std::string const& fileName)
{
    io_helpers::FileReader fileReader{ fileName };
    bgfx_memory_t const* memory = fileReader.ReadToBgfx();

    return makeUniqueShader(memory);
}


std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)> makeUniqueShader(bgfx_memory_t const* mem)
{
    bgfx_shader_handle_t shader = bgfx_create_shader(mem);

    auto custom_destroy = [](bgfx_shader_handle_t const* shaderHandle)
    {
        bgfx_destroy_shader(*shaderHandle);
    };

    return std::unique_ptr<bgfx_shader_handle_t, decltype(&uniqueShaderDestroyProxy)>{ new bgfx_shader_handle_t{ shader }, &uniqueShaderDestroyProxy };
}


std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(std::string const& fileName)
{
    io_helpers::FileReader fileReader{ fileName };
    bgfx_memory_t const* memory = fileReader.ReadToBgfx();
    
    return makeSharedShader(memory);
}

std::shared_ptr<bgfx_shader_handle_t> makeSharedShader(bgfx_memory_t const* mem)
{
    bgfx_shader_handle_t shader = bgfx_create_shader(mem);
    return std::shared_ptr<bgfx_shader_handle_t>(new bgfx_shader_handle_t{ shader },
        [](bgfx_shader_handle_t* shaderHandle)
        {
            bgfx_destroy_shader(*shaderHandle);
        });
}

std::shared_ptr<bgfx_uniform_handle_t> makeSharedUniform(std::string const& name, bgfx_uniform_type type, std::uint32_t arrayElementsCount)
{
    bgfx_uniform_handle_t uniform = bgfx_create_uniform(name.c_str(), type, arrayElementsCount);
    return std::shared_ptr<bgfx_uniform_handle_t>(new bgfx_uniform_handle_t{ uniform },
        [](bgfx_uniform_handle_t* handle)
        {
            bgfx_destroy_uniform(*handle);
        });
}

} // namespace pg::bgfx_helpers

