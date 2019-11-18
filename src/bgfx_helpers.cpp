#include <bgfx_helpers.h>

#include <cassert>

#include <io_helpers.h>
#include <stb/stb_image.h>

namespace pg::bgfx_helpers
{

std::shared_ptr<bgfx_vertex_buffer_handle_t> makeSharedVertexBuffer(bgfx_memory_t const* mem, bgfx_vertex_layout_t const* layout, std::uint16_t flags)
{
    bgfx_vertex_buffer_handle_t vertexBuffer = bgfx_create_vertex_buffer(mem, layout, flags);
    return std::shared_ptr<bgfx_vertex_buffer_handle_t>(new bgfx_vertex_buffer_handle_t{ vertexBuffer },
        [](bgfx_vertex_buffer_handle_t* vertexBufferPtr)
        {
            bgfx_destroy_vertex_buffer(*vertexBufferPtr);
        });
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

std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_shader_handle_t vsHandle, bgfx_shader_handle_t fsHandle)
{
    bgfx_program_handle_t program = bgfx_create_program(vsHandle, fsHandle, false);
    return std::shared_ptr<bgfx_program_handle_t>(new bgfx_program_handle_t{ program },
        [](bgfx_program_handle_t* programHandle)
        {
            bgfx_destroy_program(*programHandle);
        });
}

std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(bgfx_memory_t const* vsMem, bgfx_memory_t const* fsMem)
{
    bgfx_shader_handle_t vs = bgfx_create_shader(vsMem);
    bgfx_shader_handle_t fs = bgfx_create_shader(fsMem);

    bgfx_program_handle_t program = bgfx_create_program(vs, fs, true);
    return std::shared_ptr<bgfx_program_handle_t>(new bgfx_program_handle_t{ program },
        [](bgfx_program_handle_t* programHandle)
        {
            bgfx_destroy_program(*programHandle);
        });
}

std::shared_ptr<bgfx_program_handle_t> makeSharedProgram(std::string const& vsFileName, std::string const& fsFileName)
{
    pg::io_helpers::FileReader vsFileReader{ vsFileName };
    pg::io_helpers::FileReader fsFileReader{ fsFileName };

    return makeSharedProgram(vsFileReader.ReadToBgfx(), fsFileReader.ReadToBgfx());
}

std::shared_ptr<bgfx_texture_handle_t> makeShared2DTexture(std::string const& fileName, int componentsCount)
{
    int width = 0, height = 0, components = 0;
    void* textureData = stbi_load(fileName.c_str(), &width, &height, &components, componentsCount);
    assert(width && height && components && "stbi failed to load texture.");

    bgfx_release_fn_t stbiFreeDelegate = (bgfx_release_fn_t)[](void* memory, void* userData) { stbi_image_free(memory); };
    bgfx_memory_t const* textureDataBgfx = bgfx_make_ref_release(textureData, width * height * components, stbiFreeDelegate, nullptr);

    bgfx_texture_format_t textureFormat = BGFX_TEXTURE_FORMAT_COUNT;
    switch (componentsCount)
    {
    case 4:
        textureFormat = BGFX_TEXTURE_FORMAT_RGBA8;
        break;
    case 3:
        textureFormat = BGFX_TEXTURE_FORMAT_RGB8;
        break;
    default:
        assert(false && "Only 3 and 4 component count is supported for textures!");
    }

    bgfx_texture_handle_t texture = bgfx_create_texture_2d(width, height, false, 1, textureFormat, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, textureDataBgfx);
    return std::shared_ptr<bgfx_texture_handle_t>(new bgfx_texture_handle_t{ texture },
        [](bgfx_texture_handle_t* textureHandle)
        {
            bgfx_destroy_texture(*textureHandle);
        });
}

std::shared_ptr<bgfx_texture_handle_t> makeShared2DTexture(std::uint32_t width, std::uint32_t height, bgfx_texture_format format, gfx::TextureUsage usage)
{
    std::uint64_t textureFlags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
    switch (usage)
    {
    case gfx::TextureUsage::Sampled:
        textureFlags |= BGFX_TEXTURE_NONE;
        break;
    case gfx::TextureUsage::RenderTarget:
        textureFlags |= BGFX_TEXTURE_RT;
        break;
    }

    return std::make_shared<bgfx_texture_handle_t>(bgfx_create_texture_2d(width, height, false, 1, format, textureFlags, nullptr));
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

