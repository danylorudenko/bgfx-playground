#pragma once

#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/mat4x4.hpp>

namespace pg
{

struct View
{
    std::uint16_t x;
    std::uint16_t y;
    std::uint16_t width;
    std::uint16_t height;
};

class Camera
{
public:
    Camera();
    Camera(Camera const& rhs);
    Camera(Camera&& rhs);

    Camera& operator=(Camera const& rhs);
    Camera& operator=(Camera&& rhs);

    ~Camera();

    inline glm::vec3&           GetPosition() { return m_Position; }
    inline glm::vec3 const&     GetPosition() const { return m_Position; }
    inline void                 SetPosition(glm::vec3 const& pos) { m_Position = pos; }

    inline glm::quat&           GetRotation() { return m_Rotation; }
    inline glm::quat const&     GetRotation() const { return m_Rotation; }
    inline void                 SetRotation(glm::quat const& rot) { m_Rotation = rot; }

    inline float                GetFOV() const { return m_FOV; }
    inline void                 SetFOV(float fov) { m_FOV = fov; }

    inline View&                GetView() { return m_View; }
    inline View const&          GetView() const { return m_View; }
    inline void                 SetView(View const& view) { m_View = view; }

    glm::mat4                   GetDefaultViewMatrix() const;
    glm::mat4                   GetDefaultProjectionMatrix() const;

private:
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    float     m_FOV;

    View      m_View;
};

} // namespace pg

