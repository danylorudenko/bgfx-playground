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
    , m_OrientationEuler{}
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

    float constexpr C_MOV_SPEED = 0.1f;
    float constexpr C_ROT_SPEED = 0.025f;

    InputSystem* inputSystem = InputSystem::GetInstance();

    if (inputSystem->GetKeyboardButtonJustReleased(Keys::R))
    {
        m_Position = { 0.0f, 0.0f, 0.0f };
        m_OrientationEuler = { 0.0f, 0.0f, 0.0f };
    }
    
    ////////////
    // movement
    if (inputSystem->GetKeyboardButtonDown(Keys::W))
    {
        m_Position += (GetForward() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::S))
    {
        m_Position += (GetBackward() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::A))
    {
        m_Position += (GetLeft() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::D))
    {
        m_Position += (GetRight() * C_MOV_SPEED);
    }

    ////////////
    // orientation
    if (inputSystem->GetKeyboardButtonDown(Keys::Up))
    {
        m_OrientationEuler.x += -C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Down))
    {
        m_OrientationEuler.x += C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Right))
    {
        m_OrientationEuler.y += C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Left))
    {
        m_OrientationEuler.y += -C_ROT_SPEED;
    }
}

void Camera::SetRotation(glm::quat const& rotation)
{
    m_OrientationEuler = glm::eulerAngles(rotation);
}

glm::quat Camera::GetRotation() const
{
    return glm::quat{ m_OrientationEuler };
}

glm::vec3 Camera::GetForward() const
{
    return static_cast<glm::vec3>(glm::mat4_cast(GetRotation()) * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f });
}

glm::vec3 Camera::GetBackward() const
{
    return -GetForward();
}

glm::vec3 Camera::GetRight() const
{
    return glm::cross({ 0.0f, 1.0f, 0.0f }, GetForward());
}

glm::vec3 Camera::GetLeft() const
{
    return -GetRight();
}

glm::mat4 Camera::GetDefaultViewMatrix() const
{
    glm::vec3 const forwardVector = GetForward();
    glm::vec3 constexpr upVector{ 0.0f, 1.0f, 0.0f };

    return glm::lookAtLH(m_Position, m_Position + forwardVector, upVector);
}

glm::mat4 Camera::GetDefaultProjectionMatrix() const
{
    return glm::perspectiveLH(m_FOV, static_cast<float>(gfx::settings::g_MainResolutionX) / gfx::settings::g_MainResolutionY, 0.1f, 100.0f);
}

} // namespace pg

