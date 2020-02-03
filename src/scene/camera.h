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

    void                        Update(float dt);

    inline glm::vec3&           GetPosition() { return m_Position; }
    inline glm::vec3 const&     GetPosition() const { return m_Position; }
    inline void                 SetPosition(glm::vec3 const& pos) { m_Position = pos; }

    glm::quat                   GetRotation() const;
    void                        SetRotation(glm::quat const& rot);

    inline glm::vec3&           GetOrientationEuler() { return m_OrientationEuler; }
    inline glm::vec3 const&     GetOrientationEuler() const { return m_OrientationEuler;}
    inline void                 SetOrientationEuler(glm::vec3 const& orientation) { m_OrientationEuler = orientation; }

    glm::vec3                   GetForward() const;
    glm::vec3                   GetBackward() const;
    glm::vec3                   GetRight() const;
    glm::vec3                   GetLeft() const;

    inline float                GetFOV() const { return glm::degrees(m_FOV); }
    inline void                 SetFOV(float fov) { m_FOV = glm::radians(fov); }

    inline View&                GetView() { return m_View; }
    inline View const&          GetView() const { return m_View; }
    inline void                 SetView(View const& view) { m_View = view; }

    glm::mat4                   GetDefaultViewMatrix() const;
    glm::mat4                   GetDefaultProjectionMatrix() const;

private:
    glm::vec3 m_Position;
    glm::vec3 m_OrientationEuler;
    float     m_FOV;

    View      m_View;
};

} // namespace pg

