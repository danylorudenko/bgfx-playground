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


struct Vertex
{
    float pos[3];
    float color[4];
    float uv[2];
};

Vertex g_Vertices[] =
{
     Vertex{ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
     Vertex{  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f },
     Vertex{  0.0f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f }
};

} // namespace pg


int main()
{
    using namespace pg;

    HINSTANCE hInstance = GetModuleHandleA(NULL);

    Application* application = Application::CreateInstance("Playground", 800u, 600u, &MyProcHandler);

    bgfx_init_t initStruct = pg::struct_helpers::bgfxInitDefault();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");

    {
        gfx::SharedShaderProgram mainProgram = std::make_shared<gfx::ShaderProgram>("shaders\\vs_triangle.bin", "shaders\\fs_triangle.bin");

        gfx::SharedVertexLayout vertexLayout = std::make_shared<gfx::VertexLayout>();
        vertexLayout->AddAtribute(BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
        vertexLayout->AddAtribute(BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_FLOAT, false);
        vertexLayout->AddAtribute(BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false);
        vertexLayout->Bake();

        gfx::SharedVertexBuffer vertexBuffer = std::make_shared<gfx::VertexBuffer>(
            vertexLayout,
            static_cast<std::uint32_t>(sizeof(g_Vertices) / sizeof(g_Vertices[0])), 
            g_Vertices, static_cast<std::uint32_t>(sizeof(g_Vertices))
        );

        /////////////////
        // Scene

        Scene* scene = application->GetMainScene();
        Camera& mainCamera = scene->GetMainCamera();

        mainCamera.SetFOV(60.0f);
        mainCamera.SetPosition(glm::vec3{ 0.0f, 0.0f, 10.0f });
        mainCamera.SetRotation(glm::identity<glm::quat>());

        View cameraView;
        cameraView.x = 0;
        cameraView.y = 0;
        cameraView.width = gfx::settings::g_MainResolutionX;
        cameraView.height = gfx::settings::g_MainResolutionY;
        mainCamera.SetView(cameraView);


        Entity& rootEntity = scene->GetRootEntityRef();

        Entity* testEntity = rootEntity.AddChild("testEntity");
        testEntity->InitRenderableComponent(mainProgram, vertexBuffer);

        /////////////////

        pg::mainLoop();
    }

    Application::DestroyInstance();

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
        }
    }
}

void mainUpdate()
{
    Application::GetInstance()->GetMainRenderer()->Update();
}

} // namespace pg

