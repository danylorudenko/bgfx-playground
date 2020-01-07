#include <scene/camera.h>

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <gfx/gfx_settings.h>
#include <input/input_system.h>

#include <iostream>

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

void Camera::Update(float dt)
{
    using namespace input;

    float constexpr C_SPEED = 0.1f;

    InputSystem* inputSystem = InputSystem::GetInstance();
    if (inputSystem->GetKeyboardButtonJustReleased(Keys::R))
    {
        m_Position = { 0.0f, 0.0f, 0.0f };
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::W))
    {
        m_Position += (GetForward() * C_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::S))
    {
        m_Position += (GetBackward() * C_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::A))
    {
        m_Position += (GetLeft() * C_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::D))
    {
        m_Position += (GetRight() * C_SPEED);
    }
}

glm::vec3 Camera::GetForward() const
{
    return static_cast<glm::vec3>(glm::mat4_cast(m_Rotation) * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f });
}

glm::vec3 Camera::GetBackward() const
{
    return -GetForward();
}

glm::vec3 Camera::GetRight() const
{
    return glm::cross(glm::vec3{ 0.0f, 1.0f, 0.0f }, GetForward());
}

glm::vec3 Camera::GetLeft() const
{
    return -GetRight();
}

glm::mat4 Camera::GetDefaultViewMatrix() const
{
    glm::vec3 const forwardVector   = GetForward();
    glm::vec3 const rightVector     = GetRight();

    glm::vec3 const upVector = glm::cross(forwardVector, rightVector);

    return glm::lookAtLH(m_Position, m_Position + forwardVector, upVector);
}

glm::mat4 Camera::GetDefaultProjectionMatrix() const
{
    return glm::perspectiveLH(m_FOV, static_cast<float>(gfx::settings::g_MainResolutionX) / gfx::settings::g_MainResolutionY, 0.1f, 100.0f);
}

} // namespace pg

