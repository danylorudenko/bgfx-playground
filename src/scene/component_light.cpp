#include <scene/component_light.h>

#include <glm/gtc/matrix_transform.hpp>

namespace pg
{

/////////////////////////////////////
LightComponent::LightComponent()
    : m_Color{ 1.0f, 1.0f, 1.0f }
    , m_Intensity{ 1.0f }
{}

/////////////////////////////////////
DirectionalLightComponent::DirectionalLightComponent()
    : LightComponent{}
    , m_GlobalDirection{ 0.0f, -1.0f, 1.0f }
    , m_ViewWidth{ 100.0f }
{}

glm::mat4 DirectionalLightComponent::GetViewMatrix(glm::vec3 const& pos) const
{
    glm::vec3 constexpr upVector{ 0.0f, 1.0f, 0.0f };
    return glm::lookAtLH(pos - (m_GlobalDirection * m_ViewWidth), pos + m_GlobalDirection, upVector);
}

glm::mat4 DirectionalLightComponent::GetProjectionMatrix() const
{
    float const viewExpansion = m_ViewWidth;
    return glm::orthoLH(-viewExpansion, viewExpansion, -viewExpansion, viewExpansion, -viewExpansion, viewExpansion * 2);
}

void DirectionalLightComponent::SetDirection(glm::vec3 const& direction)
{
    m_GlobalDirection = glm::normalize(direction);
}

glm::vec3 const& DirectionalLightComponent::GetDirection() const
{
    return m_GlobalDirection;
}

/////////////////////////////////////
SpotLightComponent::SpotLightComponent()
    : LightComponent{}
    , m_Direction{ 0.0f, -1.0f, 0.0f }
    , m_MaxDistance{ 50.0f }
    , m_FalloffDistance{ 10.0f }
{}

glm::mat4 SpotLightComponent::GetViewMatrix(glm::vec3 const& pos) const
{
    return glm::identity<glm::mat4>();
}

glm::mat4 SpotLightComponent::GetProjectionMatrix() const
{
    return glm::identity<glm::mat4>();
}

}

