#include <bgfx_helpers.h>

#include <cassert>

#include <io_helpers.h>
#include <stb/stb_image.h>

namespace pg::bgfx_helpers
{

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

