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
    glm::vec3       upVector      = glm::vec3{ 0.0f, 1.0f, 0.0f };
    glm::vec3 const forwardVector = static_cast<glm::vec3>(glm::mat4_cast(m_Rotation) * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f });
    glm::vec3 const rightVector   = glm::cross(forwardVector, upVector);

    upVector = glm::cross(rightVector, forwardVector);

    return glm::lookAtRH(m_Position, forwardVector, upVector);
}

glm::mat4 Camera::GetDefaultProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_FOV), static_cast<float>(gfx::settings::g_MainResolutionX) / gfx::settings::g_MainResolutionY, 0.1f, 1000.0f);
}

} // namespace pg

