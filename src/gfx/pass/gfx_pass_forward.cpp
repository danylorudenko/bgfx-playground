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
    , m_UniformMVP{ "myTransforms", BGFX_UNIFORM_TYPE_MAT4, 6 }
{
}

PassForward::PassForward(PassForward&& rhs)
    : PassBase{ std::move(rhs) }
{
}

PassForward& PassForward::operator=(PassForward&& rhs)
{
    PassBase::operator=(std::move(rhs));

    std::swap(m_UniformMVP, rhs.m_UniformMVP);

    return *this;
}

PassForward::~PassForward()
{
}

void PassForward::Render(Scene* scene)
{
    PassBase::Render(scene);

    ////////////////////////////////

    PassId const passId = m_PassId;
    bgfx_set_view_name(passId, "PassForward");

    ////////////////////////////////

    bgfx_frame_buffer_handle_t const framebuffer = m_Framebuffer.Ref();
    if (BGFX_HANDLE_IS_VALID(framebuffer))
    {
        bgfx_set_view_frame_buffer(passId, framebuffer);
    }

    ////////////////////////////////

    {
        bgfx_set_view_clear(passId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f, 0);
        bgfx_set_view_rect(passId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);
        bgfx_set_view_scissor(passId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);

        // in this call we can set camera data (and view and projection transforms)
        

        //bgfx_set_view_transform(passId, glm::value_ptr(view), glm::value_ptr(proj));
    }

    Camera const& camera = scene->GetMainCamera();
    glm::mat4 const view = camera.GetDefaultViewMatrix();
    glm::mat4 const proj = camera.GetDefaultProjectionMatrix();

    {
        bgfx_touch(0);

        bgfx_set_state(0 
            | BGFX_STATE_WRITE_RGB 
            | BGFX_STATE_WRITE_A 
            | BGFX_STATE_WRITE_Z 
            | BGFX_STATE_DEPTH_TEST_LESS 
            | BGFX_STATE_CULL_CCW // we have a cube with CCW vertices
            | BGFX_STATE_MSAA, 0);

        UniformProxy* myUniform = &m_UniformMVP;

        auto entityDelegate = [passId, myUniform, view, proj](Entity& entity)
        {
            // 1. set model uniform
            // 2. set vertex index buffer
            // 3. set texture (optional)
            //glm::mat4 const modelMatrix = entity.GetGlobalModelMatrix();

            // scale, rotation, translation, view, perspective
            glm::mat4 transforms[6];

            transforms[0] = glm::scale(glm::identity<glm::mat4>(), entity.GetGlobalScale());
            transforms[1] = glm::mat4_cast(entity.GetGlobalRotation());
            transforms[2] = glm::translate(glm::identity<glm::mat4>(), entity.GetGlobalPosition());
            transforms[3] = view;
            transforms[4] = proj;
            transforms[5] = entity.GetGlobalModelMatrix();

            RenderableComponent const& renderableComponent = entity.GetRenderableComponentRef();
            VertexBuffer const& vertexBuffer = *renderableComponent.m_VertexBuffer;

            //myUniform->SetData(transforms, 6);

            bgfx_set_transform(glm::value_ptr(transforms[5]), 1);
            bgfx_set_view_transform(passId, glm::value_ptr(transforms[3]), glm::value_ptr(transforms[4]));

            bgfx_set_vertex_buffer(0, vertexBuffer.GetHandle(), 0, vertexBuffer.GetVertexCount());

            bgfx_program_handle_t const program = renderableComponent.m_ShaderProgram->GetHandle();
            bgfx_submit(passId, program, 0, true);
        };

        scene->ForEachEntity(entityDelegate);
    }

    ////////////////////////////////

}

}

