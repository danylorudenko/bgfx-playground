#pragma once

#include <queue>

#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class Renderer
    : public pg::class_features::NonCopyable
    , public pg::class_features::StaticSingleton<Renderer>
{
public:
    Renderer();

    Renderer(Renderer&& rhs);
    Renderer& operator=(Renderer&& rhs);

    ~Renderer();


    void Update(float dt);

private:
    std::vector<std::unique_ptr<PassBase>> m_PassQueue;
};

} // namespace pf::gfx

