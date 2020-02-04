#pragma once

#include <gfx/gfx_objects.h>
#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class PassDebugView;

class Renderer
    : public pg::class_features::NonCopyable
    , public pg::class_features::StaticSingleton<Renderer>
{
public:
    Renderer();

    Renderer(Renderer&& rhs);
    Renderer& operator=(Renderer&& rhs);

    ~Renderer();

    void InitRenderGraph();

    void Update(float dt);

    void RegisterShader(ShaderProgram* program);
    void UnregisterShader(ShaderProgram* program);
    void ReloadAllShaders();

private:
    std::vector<std::unique_ptr<PassBase>> m_PassQueue;
    std::vector<ShaderProgram*> m_LoadedShaders;

    PassDebugView* m_DebugView;
};

} // namespace pf::gfx

