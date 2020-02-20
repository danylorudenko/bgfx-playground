#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace pg
{

class Transformable
{
public:
    Transformable(Transformable* parent = nullptr,
        glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::quat rotation = glm::identity<glm::quat>(),
        glm::vec3 scale = glm::vec3{ 1.0f, 1.0f, 1.0f });

    Transformable(Transformable const& rhs);
    Transformable(Transformable&& rhs);

    Transformable& operator=(Transformable const& rhs);
    Transformable& operator=(Transformable&& rhs);

    virtual ~Transformable();


    Transformable const*                GetTransformableParent() const;
    Transformable*                      GetTransformableParent();

    glm::vec3 const&                    GetPosition() const;
    glm::quat const&                    GetRotation() const;
    glm::vec3                           GetOrientationEuler() const;
    glm::vec3 const&                    GetScale() const;

    void                                SetPosition(glm::vec3 const& pos);
    void                                SetRotation(glm::quat const& rot);
    void                                SetOrientationEuler(glm::vec3 const& eulerOrientation);
    void                                SetScale(glm::vec3 const& scale);

    glm::vec3                           GetGlobalPosition() const;
    glm::quat                           GetGlobalRotation() const;
    glm::vec3                           GetGlobalOrientationEuler() const;
    glm::vec3                           GetGlobalScale() const;

    void                                SetGlobalPosition(glm::vec3 const& pos);
    void                                SetGlobalRotation(glm::quat const& rot);
    void                                SetGlobalOrientationEuler(glm::vec3 const& eulerOrientation);
    void                                SetGlobalScale(glm::vec3 const& scale);


    glm::vec3                           GetForward() const;
    glm::vec3                           GetBackward() const;
    glm::vec3                           GetRight() const;
    glm::vec3                           GetLeft() const;

    // Graphics
    glm::mat4                           GetGlobalModelMatrix() const;

private:
    Transformable* m_Parent;

    glm::vec3   m_RelativePos;
    glm::quat   m_RelativeRotation;
    glm::vec3   m_RelativeScale;
};

} // namespace pg

