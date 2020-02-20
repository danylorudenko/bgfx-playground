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
    , m_RawDataProxy{ "u_RAW", BGFX_UNIFORM_TYPE_MAT4, 2 }
    , m_TextureProxy{ "u_Texture", BGFX_UNIFORM_TYPE_SAMPLER, 1 }
{
}

PassForward::PassForward(PassForward&& rhs) = default;

PassForward& PassForward::operator=(PassForward&& rhs) = default;

PassForward::~PassForward() = default;

void PassForward::Render(Scene* scene)
{
    PassBase::Render(scene);

    ////////////////////////////////

    PassId const passId = m_PassId;
    bgfx_set_view_name(passId, "PassForward");

    ////////////////////////////////

    {
        bgfx_set_view_clear(passId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f, 0);
        bgfx_set_view_rect(passId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);
        bgfx_set_view_scissor(passId, 0, 0, gfx::settings::g_MainResolutionX, gfx::settings::g_MainResolutionY);

        // in this call we can set camera data (and view and projection transforms)
        Camera const& camera = scene->GetMainCamera();
        glm::mat4 const view = camera.GetDefaultViewMatrix(camera.GetGlobalPosition(), camera.GetForward());
        glm::mat4 const proj = camera.GetDefaultProjectionMatrix();

        bgfx_set_view_transform(passId, glm::value_ptr(view), glm::value_ptr(proj));
    }

    {
        auto entityDelegate = [this, passId](Entity& entity)
        {
            if (entity.IsRenderable())
            {
                RenderableComponent& renderableComponent = entity.GetRenderableComponentRef();

                glm::mat4 const modelMatrix = entity.GetGlobalModelMatrix();
                bgfx_set_transform(glm::value_ptr(modelMatrix), 1);

                VertexBuffer const& vertexBuffer = *renderableComponent.m_VertexBuffer;
                bgfx_set_vertex_buffer(0, vertexBuffer.GetHandle(), 0, vertexBuffer.GetVertexCount());

                if (IndexBuffer const* indexBuffer = renderableComponent.m_IndexBuffer.get())
                {
                    bgfx_set_index_buffer(indexBuffer->GetHandle(), 0, indexBuffer->GetIndexCount());
                }

                renderableComponent.m_RawData.SendData(m_RawDataProxy);
                renderableComponent.m_TextureData.SendData(m_TextureProxy);

                bgfx_set_state(0
                    | BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_A
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    | BGFX_STATE_CULL_CCW // we are using left-handed system
                    | BGFX_STATE_MSAA, 0);


                bgfx_program_handle_t const program = renderableComponent.m_ShaderProgram->GetHandle();
                bgfx_submit(passId, program, 0, false);
            }
            
        };

        scene->ForEachEntity(entityDelegate);
    }

    ////////////////////////////////

}

}

