#include <scene/camera.h>

#include <input/input_system.h>

namespace pg
{

Camera::Camera() = default;

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

    if (inputSystem->GetKeyboardButtonJustReleased(Keys::C))
    {
        SetPosition({ 0.0f, 0.0f, 0.0f });
        SetOrientationEuler({ 0.0f, 0.0f, 0.0f });
    }
    
    ////////////
    // movement
    glm::vec3 position = GetPosition();
    if (inputSystem->GetKeyboardButtonDown(Keys::W))
    {
        position + (GetForward() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::S))
    {
        position += (GetBackward() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::A))
    {
        position += (GetLeft() * C_MOV_SPEED);
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::D))
    {
        position += (GetRight() * C_MOV_SPEED);
    }
    SetPosition(position);


    ////////////
    // orientation
    glm::vec3 orientation = GetOrientationEuler();
    if (inputSystem->GetKeyboardButtonDown(Keys::Up))
    {
        orientation.x += -C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Down))
    {
        orientation.x += C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Right))
    {
        orientation.y += C_ROT_SPEED;
    }
    if (inputSystem->GetKeyboardButtonDown(Keys::Left))
    {
        orientation.y += -C_ROT_SPEED;
    }
    SetOrientationEuler(orientation);
}

} // namespace pg

