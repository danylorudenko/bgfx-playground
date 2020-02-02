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
    , m_GlobalDirection{ 0.0f, -1.0f, 0.0f }
    , m_ViewWidth{ 600.0f }
{}

glm::mat4 DirectionalLightComponent::GetViewMatrix() const
{
    return glm::identity<glm::mat4>();
}

glm::mat4 DirectionalLightComponent::GetProjectionMatrix() const
{
    return glm::identity<glm::mat4>();
}

/////////////////////////////////////
SpotLightComponent::SpotLightComponent()
    : LightComponent{}
    , m_Direction{ 0.0f, -1.0f, 0.0f }
    , m_MaxDistance{ 50.0f }
    , m_FalloffDistance{ 10.0f }
{}

glm::mat4 SpotLightComponent::GetViewMatrix() const
{
    //float const viewExpansion = m_ViewWidth / 2;
    //
    //glm::vec3 const forwardVector = GetForward();
    //glm::vec3 constexpr upVector{ 0.0f, 1.0f, 0.0f };
    //
    //glm::mat4 const view = glm::lookAtLH(m_Position, m_Position + forwardVector, upVector);
    //glm::mat4 const proj = glm::orthoLH(viewExpansion, viewExpansion, viewExpansion, viewExpansion, 0.0f, m_ViewWidth);
    //
    //return view * proj;
    return glm::identity<glm::mat4>();
}

glm::mat4 SpotLightComponent::GetProjectionMatrix() const
{
    return glm::identity<glm::mat4>();
}

}

