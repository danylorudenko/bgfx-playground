#include <cassert>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <bgfx/c99/bgfx.h>

#include <bgfx_helpers.h>
#include <gfx/gfx_objects.h>
#include <gfx/gfx_settings.h>
#include <application.h>
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
};

Vertex g_Vertices[] =
{
    Vertex{ -1.0f,-1.0f,-1.0f, 0.0f, 0.7f, 0.5f, 1.0f },// triangle 1 : begin
    Vertex{ -1.0f,-1.0f, 1.0f, 1.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f },// triangle 1 : end
    Vertex{ 1.0f, 1.0f,-1.0f,  0.0f, 0.7f, 0.5f, 1.0f },// triangle 2 : begin
    Vertex{ -1.0f,-1.0f,-1.0f, 1.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f, 1.0f,-1.0f, 0.2f, 0.7f, 0.5f, 1.0f },// triangle 2 : end
    Vertex{ 1.0f,-1.0f, 1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f,-1.0f,-1.0f, 0.0f, 0.7f, 0.4f, 1.0f },
    Vertex{ 1.0f,-1.0f,-1.0f,  0.0f, 0.1f, 0.5f, 1.0f },
    Vertex{ 1.0f, 1.0f,-1.0f,  0.2f, 0.9f, 0.5f, 1.0f },
    Vertex{ 1.0f,-1.0f,-1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f,-1.0f,-1.0f, 0.0f, 0.4f, 0.8f, 1.0f },
    Vertex{ -1.0f,-1.0f,-1.0f, 0.0f, 0.7f, 0.8f, 1.0f },
    Vertex{ -1.0f, 1.0f, 1.0f, 0.0f, 0.4f, 0.5f, 1.0f },
    Vertex{ -1.0f, 1.0f,-1.0f, 0.2f, 0.7f, 0.5f, 1.0f },
    Vertex{ 1.0f,-1.0f, 1.0f,  0.1f, 0.5f, 0.5f, 1.0f },
    Vertex{ -1.0f,-1.0f, 1.0f, 0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f,-1.0f,-1.0f, 0.0f, 0.7f, 0.6f, 1.0f },
    Vertex{ -1.0f, 1.0f, 1.0f, 0.1f, 0.7f, 0.0f, 1.0f },
    Vertex{ -1.0f,-1.0f, 1.0f, 0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ 1.0f,-1.0f, 1.0f,  1.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ 1.0f, 1.0f, 1.0f,  0.0f, 0.2f, 0.0f, 1.0f },
    Vertex{ 1.0f,-1.0f,-1.0f,  0.0f, 0.7f, 0.3f, 1.0f },
    Vertex{ 1.0f, 1.0f,-1.0f,  1.0f, 0.1f, 0.5f, 1.0f },
    Vertex{ 1.0f,-1.0f,-1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ 1.0f, 1.0f, 1.0f,  1.0f, 0.7f, 0.7f, 1.0f },
    Vertex{ 1.0f,-1.0f, 1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ 1.0f, 1.0f, 1.0f,  0.1f, 0.5f, 0.7f, 1.0f },
    Vertex{ 1.0f, 1.0f,-1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f, 1.0f,-1.0f, 0.0f, 0.4f, 0.5f, 1.0f },
    Vertex{ 1.0f, 1.0f, 1.0f,  0.4f, 0.7f, 1.0f, 1.0f },
    Vertex{ -1.0f, 1.0f,-1.0f, 0.9f, 0.1f, 0.0f, 1.0f },
    Vertex{ -1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 1.5f, 1.0f },
    Vertex{ 1.0f, 1.0f, 1.0f,  0.0f, 0.7f, 0.5f, 1.0f },
    Vertex{ -1.0f, 1.0f, 1.0f, 0.6f, 0.4f, 1.0f, 1.0f },
    Vertex{ 1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f }
};

} // namespace pg

pg::Entity* g_TestEntity = nullptr;

int main()
{
    using namespace pg;

    HINSTANCE hInstance = GetModuleHandleA(NULL);

    // ==================
    gfx::settings::g_MainResolutionX = 600;
    gfx::settings::g_MainResolutionY = 600;
    gfx::settings::g_BackbufferFormat = BGFX_TEXTURE_FORMAT_RGBA8;
    // ==================

    Application* application = Application::CreateInstance("Playground", &MyProcHandler);
    gfx::Renderer* renderer = gfx::Renderer::CreateInstance();
    Scene* scene = Scene::CreateInstance();

    {
        gfx::SharedShaderProgram mainProgram = std::make_shared<gfx::ShaderProgram>("shaders\\vs_triangle.bin", "shaders\\fs_triangle.bin");

        gfx::SharedVertexLayout vertexLayout = std::make_shared<gfx::VertexLayout>();
        vertexLayout->AddAtribute(BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false);
        vertexLayout->AddAtribute(BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_FLOAT, false);
        vertexLayout->Bake();

        gfx::SharedVertexBuffer vertexBuffer = std::make_shared<gfx::VertexBuffer>(
            vertexLayout,
            static_cast<std::uint32_t>(sizeof(g_Vertices) / sizeof(g_Vertices[0])), 
            g_Vertices, static_cast<std::uint32_t>(sizeof(g_Vertices))
        );

        /////////////////
        // Scene

        Camera& mainCamera = scene->GetMainCamera();

        mainCamera.SetFOV(60.0f);
        mainCamera.SetPosition(glm::vec3{ 0.0f, 0.0f, 0.0f });
        mainCamera.SetRotation(glm::identity<glm::quat>());

        View cameraView;
        cameraView.x = 0;
        cameraView.y = 0;
        cameraView.width = gfx::settings::g_MainResolutionX;
        cameraView.height = gfx::settings::g_MainResolutionY;
        mainCamera.SetView(cameraView);


        Entity& rootEntity = scene->GetRootEntityRef();

        g_TestEntity = rootEntity.AddChild("testEntity");
        g_TestEntity->InitRenderableComponent(mainProgram, vertexBuffer);
        g_TestEntity->SetPosition({ 0.0f, 0.0f, 15.0f });
        g_TestEntity->SetScale({ 1.2f, 1.2f, 1.2f });

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


float g_CounterPos = 0.0f;
float g_CounterScale = 0.0f;
float g_CounterRot = 0.0f;
float g_CounterCamOrbit = 0.0f;

void mainUpdate()
{
    g_TestEntity->SetRotation(glm::quat{ { g_CounterRot += 0.02f, 0.0f, 0.0f } });
    //g_TestEntity->SetPosition({ glm::sin(g_CounterPos += 0.01f), 0.0f, 0.0f });
    //g_TestEntity->SetScale({ 0.2f, glm::abs(glm::sin(g_CounterScale += 0.05f)) * 0.2f, 0.2f });

    ///////////////////////

    Camera& cam = Scene::GetInstance()->GetMainCamera();
    float constexpr C_ORBIT_RADIUS = 10.5;

    g_CounterCamOrbit += 0.01f;
    cam.SetPosition({ glm::sin(g_CounterCamOrbit) * C_ORBIT_RADIUS, 0.0f, glm::cos(g_CounterCamOrbit) * C_ORBIT_RADIUS });
    //cam.SetRotation(glm::quat{{ 0.0f, 0.0f, 0.0f }});


    gfx::Renderer::GetInstance()->Update();
}

} // namespace pg

