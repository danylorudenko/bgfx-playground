#include <gfx/gfx_renderer.h>

#include <algorithm>
#include <cassert>

#include <bgfx/c99/bgfx.h>
#include <gfx/bgfx_helpers.h>
#include <scene/scene.h>

#include <gfx/pass/gfx_pass_debugview.h>
#include <gfx/pass/gfx_pass_forward.h>
#include <gfx/pass/gfx_pass_shadow.h>

namespace pg::gfx
{

Renderer::Renderer()
    : m_DebugView{ nullptr }
{
    bgfx_init_t initStruct = pg::bgfx_helpers::bgfxInitDefault();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");
}

Renderer::Renderer(Renderer&&) = default;

Renderer& Renderer::operator=(Renderer&& rhs) = default;

Renderer::~Renderer() = default;

void Renderer::InitRenderGraph()
{
    m_PassQueue.emplace_back(std::make_unique<PassShadow>(PassIdKey::kShadow));
    m_PassQueue.emplace_back(std::make_unique<PassForward>(PassIdKey::kForward));

    m_PassQueue.emplace_back(std::make_unique<PassDebugView>(PassIdKey::kDebugView));
    m_DebugView = reinterpret_cast<PassDebugView*>(m_PassQueue.back().get());
}

void Renderer::Update(float dt)
{
    bgfx_touch(0);

    PassId scheduleGuard = PassId{};
    for (auto& pass : m_PassQueue)
    {
        PassId passId = pass->GetPassId();
        assert((int)passId > (int)scheduleGuard && "Pass ordering schedule is violated.");
        scheduleGuard = scheduleGuard;

        pass->Render(Scene::GetInstance());
    }

    bgfx_frame(false);
}

void Renderer::ReloadAllShaders()
{
    for (ShaderProgram* program : m_LoadedShaders)
    {
        program->Reload();
    }
}

void Renderer::RegisterShader(ShaderProgram* program)
{
    m_LoadedShaders.emplace_back(program);
}

void Renderer::UnregisterShader(ShaderProgram* program)
{
    auto eraseBegin = std::remove(m_LoadedShaders.begin(), m_LoadedShaders.end(), program);
    m_LoadedShaders.erase(eraseBegin, m_LoadedShaders.end());
}


}

