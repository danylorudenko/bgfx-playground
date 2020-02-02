#include <gfx/gfx_renderer.h>

#include <algorithm>
#include <cassert>

#include <bgfx/c99/bgfx.h>
#include <gfx/bgfx_helpers.h>
#include <scene/scene.h>

#include <gfx/pass/gfx_pass_forward.h>
#include <gfx/pass/gfx_pass_shadow.h>

namespace pg::gfx
{

Renderer::Renderer()
{
    bgfx_init_t initStruct = pg::bgfx_helpers::bgfxInitDefault();
    bool result = bgfx_init(&initStruct);
    assert(result && "bgfx failed to initialize!");

    //m_PassQueue.emplace_back(std::make_unique<PassShadow>(PassId::kShadow));
    m_PassQueue.emplace_back(std::make_unique<PassForward>(PassId::kForward));
}

Renderer::Renderer(Renderer&&) = default;

Renderer& Renderer::operator=(Renderer&& rhs) = default;

Renderer::~Renderer() = default;

void Renderer::Update(float dt)
{
    bgfx_touch(0);

    PassId scheduleGuard = PassId::kNone;
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
    std::remove(m_LoadedShaders.begin(), m_LoadedShaders.end(), program);
}


}

