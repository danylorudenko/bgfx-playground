#include <gfx/pass/gfx_pass_forward.h>

#include <utility>

#include <scene/camera.h>
#include <scene/scene.h>
#include <gfx/gfx_settings.h>
#include <glm/gtc/type_ptr.hpp>


namespace pg::gfx
{

PassForward::PassForward(PassId scheduleId)
    : PassBase{ scheduleId }
{}

PassForward::PassForward(PassForward&& rhs)
    : PassBase{ std::move(rhs) }
{
}

PassForward& PassForward::operator=(PassForward&& rhs)
{
    PassBase::operator=(std::move(rhs));

    return *this;
}

PassForward::~PassForward()
{
}

void PassForward::Begin(Scene* scene)
{
    PassBase::Begin(scene);

    ////////////////////////////////

    bgfx_set_view_name(m_PassId, "PassForward");

    ////////////////////////////////

    bgfx_frame_buffer_handle_t const framebuffer = m_Framebuffer.Ref();
    if (BGFX_HANDLE_IS_VALID(framebuffer))
    {
        bgfx_set_view_frame_buffer(m_PassId, framebuffer);
    }

    ////////////////////////////////

    {
        bgfx_set_view_clear(m_PassId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
        bgfx_set_view_rect(m_PassId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);
        bgfx_set_view_scissor(m_PassId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);

        // in this call we can set camera data (and view and projection transforms)
        Camera const& camera = scene->GetMainCamera();
        glm::mat4 const view = camera.GetDefaultViewMatrix();
        glm::mat4 const proj = camera.GetDefaultProjectionMatrix();

        bgfx_set_view_transform(m_PassId, glm::value_ptr(view), glm::value_ptr(proj));
    }

    {
        bgfx_touch(m_PassId);

        Scene::EntityGenericDelegate entityDelegate = [](Entity& entity)
        {
            // 1. set model uniform
            // 2. set vertex index buffer
            // 3. set texture (optional)

            bgfx_set_transform
        };

        scene->ForEachEntity(entityDelegate);
    }

    ////////////////////////////////

}

void PassForward::Render(Scene* scene)
{
    PassBase::Render(scene);
}

}

