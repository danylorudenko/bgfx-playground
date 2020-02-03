#include <gfx/pass/gfx_pass_shadow.h>

#include <bgfx/c99/bgfx.h>
#include <glm/gtc/type_ptr.hpp>

#include <scene/scene.h>

namespace pg::gfx
{

constexpr std::uint16_t C_SHADOWMAP_DIMENTIONS[2] = { 600, 600 };

PassShadow::PassShadow(PassId passId)
    : PassBase{ passId }
    , m_Framebuffer{}
    , m_ShadowMap{ std::make_shared<Texture>(TextureUsage::DepthReadWrite, C_SHADOWMAP_DIMENTIONS[0], C_SHADOWMAP_DIMENTIONS[1], BGFX_TEXTURE_FORMAT_D16) }
    , m_ShadowGenProgram{ std::make_shared<ShaderProgram>("shaders\\vs_shadowmap_gen.bin", "shaders\\fs_shadowmap_gen.bin") }
{
    bgfx_attachment_t attachmentDesc;
    bgfx_attachment_init(&attachmentDesc, m_ShadowMap->GetHandle(), BGFX_ACCESS_READWRITE, 0, 0, BGFX_RESOLVE_NONE);

    m_Framebuffer = std::make_shared<Framebuffer>(1, &attachmentDesc);
}

PassShadow::PassShadow(PassShadow&& rhs) = default;

PassShadow& PassShadow::operator=(PassShadow&& rhs) = default;

PassShadow::~PassShadow() = default;

void PassShadow::Render(Scene* scene)
{
    PassBase::Render(scene);

    ////////////////////////////////

    PassId const passId = m_PassId;
    bgfx_set_view_name(passId, "PassShadow");

    ////////////////////////////////

    DirectionalLightComponent const& mainLight = scene->GetMainDirectionalLight();

    bgfx_set_view_rect(passId, 0, 0, C_SHADOWMAP_DIMENTIONS[0], C_SHADOWMAP_DIMENTIONS[1]);
    bgfx_set_view_scissor(passId, 0, 0, C_SHADOWMAP_DIMENTIONS[0], C_SHADOWMAP_DIMENTIONS[1]);

    bgfx_set_view_frame_buffer(passId, m_Framebuffer->GetHandle());

    Camera const& mainCamera = scene->GetMainCamera();

    glm::mat4 const view = mainLight.GetViewMatrix(mainCamera.GetPosition(), mainCamera.GetForward());
    glm::mat4 const proj = mainLight.GetProjectionMatrix();
    bgfx_set_view_transform(passId, glm::value_ptr(view), glm::value_ptr(proj));

    auto entityDelegate = [passId, this](Entity& entity)
    {
        if (entity.IsRenderable())
        {
            glm::mat4 const model = entity.GetGlobalModelMatrix();
            bgfx_set_transform(glm::value_ptr(model), 1);
            bgfx_set_state(BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_CULL_CCW, 0);

            RenderableComponent const& renderableComponent = entity.GetRenderableComponentRef();
            bgfx_set_vertex_buffer(0, renderableComponent.m_VertexBuffer->GetHandle(), 0, renderableComponent.m_VertexBuffer->GetVertexCount());
            if (IndexBuffer const* indexBuffer = renderableComponent.m_IndexBuffer.get())
            {
                bgfx_set_index_buffer(indexBuffer->GetHandle(), 0, indexBuffer->GetIndexCount());
            }

            bgfx_submit(passId, m_ShadowGenProgram->GetHandle(), 0, false);
        }
    };

    scene->ForEachEntity(entityDelegate);



    /////////////////////////
    // debug render

}

}

