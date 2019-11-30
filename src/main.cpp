#include <cassert>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

#include <bgfx_helpers.h>
#include <gfx/gfx_objects.h>
#include <gfx/gfx_settings.h>
#include <globals.h>
#include <io_helpers.h>
#include <scene/entity.h>
#include <struct_helpers.h>
#include <windows/window.h>

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


bgfx_vertex_layout_t                            g_VertexLayout;
std::shared_ptr<bgfx_texture_handle_t>          g_MainTexture;
bgfx_uniform_handle_t                           g_MainTextureUniform;
bgfx_uniform_handle_t                           g_CustomPosUnifrom;

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

    bgfx_init_t initStruct = pg::struct_helpers::bgfxInitDefault();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");


    gfx::ShaderRef mainProgram{ "shaders\\vs_triangle.bin", "shaders\\fs_triangle.bin" };

    bgfx_vertex_layout_begin(&g_VertexLayout, bgfx_get_renderer_type());
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_add(&g_VertexLayout, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_layout_end(&g_VertexLayout);

    gfx::VertexBufferRef vertexBuffer{ sizeof(g_Vertices) / sizeof(g_Vertices[0]), bgfx_make_ref(g_Vertices, sizeof(g_Vertices)), &g_VertexLayout };

    g_MainTexture = pg::bgfx_helpers::makeShared2DTexture("assets\\wood.png");

    g_MainTextureUniform = bgfx_create_uniform("mainTexture", BGFX_UNIFORM_TYPE_SAMPLER, 1);
    g_CustomPosUnifrom = bgfx_create_uniform("u_customPos", BGFX_UNIFORM_TYPE_VEC4, 1);

    bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00ffff00, 1.0f, 0);

    /////////////////
    // Scene

    Camera& mainCamera = g_MainScene.GetMainCamera();

    mainCamera.SetFOV(60.0f);
    mainCamera.SetPosition(glm::vec3{ 0.0f, 0.0f, -10.0f });
    mainCamera.SetRotation(glm::identity<glm::quat>());

    View cameraView;
    cameraView.x = 0;
    cameraView.y = 0;
    cameraView.width = gfx::settings::g_MainResolutionX;
    cameraView.height = gfx::settings::g_MainResolutionY;
    mainCamera.SetView(cameraView);


    Entity& rootEntity = g_MainScene.GetRootEntityRef();
    
    Entity* testEntity = rootEntity.AddChild("testEntity");
    testEntity->InitRenderableComponent(mainProgram, vertexBuffer);

    /////////////////

    g_MainRenderer = std::make_unique<gfx::Renderer>();
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

float g_Counter = 0.0f;
void mainUpdate()
{
    //bgfx_touch(0);
    //bgfx_set_view_rect(0, 0, 0, (uint16_t)g_MainWindow.Width(), (uint16_t)g_MainWindow.Height());
    //bgfx_set_vertex_buffer(0, *g_VertexBuffer, 0, 3);
    //bgfx_set_state(BGFX_STATE_DEFAULT, 0);
    //bgfx_set_texture(0, g_MainTextureUniform, *g_MainTexture, UINT32_MAX);

    //g_Counter = g_Counter > 1.0f ? -1.0f : g_Counter + 0.01f;
    //float customPos[4] = { g_Counter, 0.5f, 0.0f, 0.0f };
    //bgfx_set_uniform(g_CustomPosUnifrom, customPos, 1);

    //bgfx_submit(0, *g_MainProgram, 0, false);

    g_MainRenderer->Update();
}

} // namespace pg

