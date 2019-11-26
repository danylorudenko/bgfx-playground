#pragma once

#include <queue>

#include <gfx/pass/gfx_passbase.h>

namespace pg::gfx
{

class Renderer
{
public:
    Renderer();

    Renderer(Renderer&& rhs);
    Renderer& operator=(Renderer&& rhs);

    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;

    ~Renderer();


    void Update();

private:
    PassId m_PassQueueGuard;
    std::vector<std::unique_ptr<PassBase>> m_PassQueue;
};

} // namespace pf::gfx

