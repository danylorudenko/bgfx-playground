#include <gfx/pass/gfx_pass_shadow.h>

#include <bgfx/c99/bgfx.h>

namespace pg::gfx
{

constexpr std::uint16_t C_SHADOWMAP_DIMENTIONS[2] = { 600, 600 };

PassShadow::PassShadow(PassId passId)
    : PassBase{ passId }
    , m_Framebuffer{ BGFX_INVALID_HANDLE }
    , m_ShadowMap{ std::make_shared<Texture>(TextureUsage::DepthReadWrite, C_SHADOWMAP_DIMENTIONS[0], C_SHADOWMAP_DIMENTIONS[1], BGFX_TEXTURE_FORMAT_D24) }
{
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

    //bgfx_set_view_transform()
}

}

