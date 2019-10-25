#include <cassert>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
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

bgfx_program_handle_t g_MainProgram;
bgfx_vertex_layout_t g_VertexLayout;
bgfx_vertex_buffer_handle_t g_VertexBuffer;
bgfx_texture_handle_t g_MainTexture;
bgfx_uniform_handle_t g_MainTextureUniform;

float g_Vertices[] =
{
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f
};

} // namespace pg


int main()
{
    using namespace pg;

    HINSTANCE hInstance = GetModuleHandleA(NULL);
    g_MainWindow = Window{ hInstance, "MyWindow", 800, 600, "MyWindowClass", &MyProcHandler, nullptr };

    bgfx_init_t initStruct = pg::bgfx_init_t_default();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");


    FileReader v_FileReader{ "shaders\\vs_triangle.bin" };
    bgfx_memory_t const* v_ShaderData = v_FileReader.ReadToBgfx();
    bgfx_shader_handle_t v_ShaderHandle = bgfx_create_shader(v_ShaderData);

    FileReader f_FileReader{ "shaders\\fs_triangle.bin" };
    bgfx_memory_t const* f_ShaderData = f_FileReader.ReadToBgfx();
    bgfx_shader_handle_t f_ShaderHandle = bgfx_create_shader(f_ShaderData);

    g_MainProgram = bgfx_create_program(v_ShaderHandle, f_ShaderHandle, false);

    bgfx_vertex_layout_begin(&g_VertexLayout, bgfx_get_renderer_type());
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_end(&g_VertexLayout);

    g_VertexBuffer = bgfx_create_vertex_buffer(bgfx_make_ref(g_Vertices, sizeof(g_Vertices)), &g_VertexLayout, BGFX_BUFFER_NONE);

    // bgfx_is_texutre_valid
    // bgfx_calc_texture_size
    // or even can use stbi_load() to load from file directly

    int width = 0, height = 0, components = 0;
    void* textureData = stbi_load("assets\\wood.png", &width, &height, &components, 3);
    assert(width && height && components && "stbi failed to load texture.");

    bgfx_release_fn_t stbiFreeDelegate = (bgfx_release_fn_t)[](void* memory, void* userData) { stbi_image_free(memory); };
    bgfx_memory_t const* textureDataBgfx = bgfx_make_ref_release(textureData, width * height * components, stbiFreeDelegate, nullptr);
    g_MainTexture = bgfx_create_texture_2d(width, height, false, 1, BGFX_TEXTURE_FORMAT_RGB8, BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, textureDataBgfx);

    g_MainTextureUniform = bgfx_create_uniform("mainTexture", BGFX_UNIFORM_TYPE_SAMPLER, 1);


    bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00ffff00, 1.0f, 0);

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
    bgfx_touch(0);
    bgfx_set_view_rect(0, 0, 0, (uint16_t)g_MainWindow.Width(), (uint16_t)g_MainWindow.Height());
    bgfx_set_vertex_buffer(0, g_VertexBuffer, 0, 3);
    bgfx_set_state(BGFX_STATE_DEFAULT, 0);
    bgfx_set_texture(0, g_MainTextureUniform, g_MainTexture, UINT32_MAX);
    bgfx_submit(0, g_MainProgram, 0, false);
}

} // namespace pg

