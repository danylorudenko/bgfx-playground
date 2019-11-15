#pragma once

#include <queue>

#include <gfx/gfx_pass.h>

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

private:
    std::queue<Pass> m_PassQueue;
};

} // namespace pf::gfx

