#include <cassert>
#include <cstdlib>
#include <iostream>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <assimp/Importer.hpp>
#include <stb/stb_image.h>
#include <bgfx/c99/bgfx.h>

#include <gfx/bgfx_helpers.h>
#include <gfx/gfx_objects.h>
#include <gfx/gfx_settings.h>
#include <input/input_system.h>
#include <io/io_helpers.h>
#include <main/application.h>
#include <scene/entity.h>
#include <system/window.h>

namespace pg
{

LRESULT MyProcHandler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_INPUT:
    {
        UINT code = GET_RAWINPUT_CODE_WPARAM(wparam);
        LRESULT result;
        if (code == RIM_INPUTSINK || code == RIM_INPUT)
            result = ::DefWindowProc(handle, message, wparam, lparam);

        input::InputSystem::GetInstance()->ProcessSystemInput(handle, wparam, lparam);
        return result;
    }
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

pg::Entity* g_TestEntity[3];

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
    input::InputSystem::CreateInstance(application->GetMainWindow()->NativeHandle());
    gfx::Renderer::CreateInstance();
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
        mainCamera.SetPosition(glm::vec3{ -10.0f, 10.0f, -10.0f });
        mainCamera.SetRotation(glm::quat({ 0.0f, -45.0f, 0.0f }));

        View cameraView;
        cameraView.x = 0;
        cameraView.y = 0;
        cameraView.width = gfx::settings::g_MainResolutionX;
        cameraView.height = gfx::settings::g_MainResolutionY;
        mainCamera.SetView(cameraView);


        Entity& rootEntity = scene->GetRootEntityRef();

        g_TestEntity[0] = rootEntity.AddChild("testEntity0");
        g_TestEntity[0]->InitRenderableComponent(mainProgram, vertexBuffer);
        g_TestEntity[0]->GetRenderableComponentRef().m_TextureData = std::make_shared<gfx::Texture>("assets\\wood.png");

        g_TestEntity[1] = g_TestEntity[0]->AddChild("testEntity1");
        g_TestEntity[1]->InitRenderableComponent(mainProgram, vertexBuffer);
        g_TestEntity[1]->GetRenderableComponentRef().m_TextureData = std::make_shared<gfx::Texture>("assets\\wood.png");
        g_TestEntity[1]->SetPosition({ 2.5f, 0.0f, 0.0f });
        g_TestEntity[1]->SetRotation(glm::quat{{ 45.0f, 0.0f, 0.0f }});

        g_TestEntity[2] = g_TestEntity[0]->AddChild("testEntity2");
        g_TestEntity[2]->InitRenderableComponent(mainProgram, vertexBuffer);
        g_TestEntity[2]->GetRenderableComponentRef().m_TextureData = std::make_shared<gfx::Texture>("assets\\wood.png");
        g_TestEntity[2]->SetPosition({ -0.5f, 0.0f, 1.5f });
        g_TestEntity[2]->SetRotation(glm::quat{ { 0.0f, 0.0f, 0.0f } });
        g_TestEntity[2]->SetScale({ 0.5f, 2.0f, 0.5f });

        /////////////////

        Entity* sponzaRoot = scene->LoadFromFile("J:\\Assets\\glTF-Sample-Models-master\\glTF-Sample-Models-master\\2.0\\Sponza\\glTF\\Sponza.gltf");
        sponzaRoot->SetPosition({ 0.0f, 0.0f, 0.0f });
        sponzaRoot->SetScale({ 0.03f, 0.03f, 0.03f });

        Entity* pbrHelmetRoot = scene->LoadFromFile("J:\\Assets\\glTF-Sample-Models-master\\glTF-Sample-Models-master\\2.0\\SciFiHelmet\\glTF\\SciFiHelmet.gltf");

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

void checkShaderReloadInput();

void mainUpdate()
{
    g_TestEntity[0]->SetRotation(g_TestEntity[0]->GetRotation() * glm::quat{ { 0.02f, 0.0f, 0.0f } });
    g_TestEntity[0]->SetPosition({ glm::sin(g_CounterPos += 0.05f) * 3.0f + 20.0f, 7.0f, 0.0f });


    ///////////////////////

    input::InputSystem::GetInstance()->Update(0.0f);
    checkShaderReloadInput();

    Scene::GetInstance()->Update(0.0f);
    gfx::Renderer::GetInstance()->Update(0.0f);
}

void checkShaderReloadInput()
{
    using namespace pg::input;

    if (InputSystem::GetInstance()->GetKeyboardButtonJustReleased(Keys::R))
    {
        assert(0 == system("..\\process_user_shaders_dx11_internal.bat") && "Failed to hot-reload shaders.");
        gfx::Renderer::GetInstance()->ReloadAllShaders();

    }
}

} // namespace pg

