#include <scene/camera.h>

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <gfx/gfx_settings.h>

namespace pg
{

Camera::Camera()
    : m_Position{}
    , m_Rotation{}
    , m_FOV{ 0.0f }
    , m_View{ 0, 0, 0, 0 }
{}

Camera::Camera(Camera const&) = default;

Camera::Camera(Camera&&) = default;

Camera& Camera::operator=(Camera const&) = default;

Camera& Camera::operator=(Camera&&) = default;

Camera::~Camera() = default;

glm::mat4 Camera::GetDefaultViewMatrix() const
{
    glm::vec3 const upVector        = glm::vec3{ 0.0f, 1.0f, 0.0f };
    glm::vec3 const forwardVector   = static_cast<glm::vec3>(glm::mat4_cast(m_Rotation) * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f });
    glm::vec3 const rightVector     = glm::cross(upVector, forwardVector);

    glm::vec3 const adjustedUpVector = glm::cross(forwardVector, rightVector);

    return glm::lookAtLH(m_Position, m_Position + forwardVector, adjustedUpVector);
}

glm::mat4 Camera::GetDefaultProjectionMatrix() const
{
    return glm::perspectiveLH(m_FOV, static_cast<float>(gfx::settings::g_MainResolutionX) / gfx::settings::g_MainResolutionY, 0.1f, 100.0f);
}

} // namespace pg

