#pragma once

#include <memory>

#include <gfx/gfx_renderer.h>
#include <scene/scene.h>
#include <windows/window.h>


namespace pg
{

extern Window g_MainWindow;

extern Scene g_MainScene;

extern std::unique_ptr<gfx::Renderer> g_MainRenderer;

} // namespace pg
