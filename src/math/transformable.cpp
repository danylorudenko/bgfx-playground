#include <math/transformable.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace pg
{

Transformable::Transformable(Transformable* parent, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    : m_Parent{ parent }
    , m_RelativePos{ 0.0f, 0.0f, 0.0f }
    , m_RelativeRotation{ glm::identity<glm::quat>() }
    , m_RelativeScale{ 1.0f, 1.0f, 1.0f }
{}

Transformable::Transformable(Transformable const& rhs) = default;

Transformable::Transformable(Transformable&& rhs) = default;

Transformable& Transformable::operator=(Transformable const& rhs) = default;

Transformable& Transformable::operator=(Transformable&& rhs) = default;

Transformable::~Transformable() = default;

Transformable const* Transformable::GetTransformableParent() const
{
    return m_Parent;
}

Transformable* Transformable::GetTransformableParent()
{
    return m_Parent;
}

glm::vec3 const& Transformable::GetPosition() const
{
    return m_RelativePos;
}

glm::quat const& Transformable::GetRotation() const
{
    return m_RelativeRotation;
}

glm::vec3 Transformable::GetOrientationEuler() const
{
    return glm::eulerAngles(m_RelativeRotation);
}

glm::vec3 const& Transformable::GetScale() const
{
    return m_RelativeScale;
}

void Transformable::SetPosition(glm::vec3 const& pos)
{
    m_RelativePos = pos;
}

void Transformable::SetRotation(glm::quat const& rot)
{
    m_RelativeRotation = rot;
}

void Transformable::SetOrientationEuler(glm::vec3 const& eulerOrientation)
{
    m_RelativeRotation = glm::quat{ eulerOrientation };
}

void Transformable::SetScale(glm::vec3 const& scale)
{
    m_RelativeScale = scale;
}

glm::vec3 Transformable::GetGlobalPosition() const
{
    return m_Parent
        ? (static_cast<glm::vec3>(glm::mat4_cast(m_Parent->GetGlobalRotation()) * glm::vec4{ m_RelativePos * m_Parent->GetGlobalScale(), 0.0f })) + m_Parent->GetGlobalPosition()
        : m_RelativePos;
}

glm::quat Transformable::GetGlobalRotation() const
{
    glm::quat result = m_RelativeRotation;
    return m_Parent ? m_RelativeRotation * m_Parent->GetGlobalRotation() : m_RelativeRotation;
}

glm::vec3 Transformable::GetGlobalOrientationEuler() const
{
    return glm::eulerAngles(GetGlobalRotation());
}

glm::vec3 Transformable::GetGlobalScale() const
{
    glm::vec3 result = m_RelativeScale;
    return m_Parent ? m_RelativeScale * m_Parent->GetGlobalScale() : m_RelativeScale;
}

void Transformable::SetGlobalPosition(glm::vec3 const& globalPos)
{
    glm::vec3 const currentPos = GetGlobalPosition();
    glm::vec3 const diff = globalPos - currentPos;

    SetPosition(GetPosition() + diff);
}

void Transformable::SetGlobalRotation(glm::quat const& globalRotation)
{
    glm::quat const currentRot = GetGlobalRotation();
    glm::quat const diff = glm::inverse(currentRot) * globalRotation;

    SetRotation(GetRotation() * diff);
}

void Transformable::SetGlobalOrientationEuler(glm::vec3 const& eulerOrientation)
{
    SetGlobalRotation(glm::quat{ eulerOrientation });
}

void Transformable::SetGlobalScale(glm::vec3 const& globalScale)
{
    glm::vec3 const currentScale = GetGlobalScale();
    glm::vec3 const diff = globalScale / currentScale;

    SetScale(GetScale() * diff);
}

glm::vec3 Transformable::GetForward() const
{
    return static_cast<glm::vec3>(glm::mat4_cast(GetRotation()) * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f });
}

glm::vec3 Transformable::GetBackward() const
{
    return -GetForward();
}

glm::vec3 Transformable::GetRight() const
{
    return glm::cross({ 0.0f, 1.0f, 0.0f }, GetForward());
}

glm::vec3 Transformable::GetLeft() const
{
    return -GetRight();
}

glm::mat4 Transformable::GetGlobalModelMatrix() const
{
    constexpr glm::mat4 identity = glm::identity<glm::mat4>();

    glm::mat4 model = glm::translate(identity, GetGlobalPosition());
    model = model * glm::mat4_cast(GetGlobalRotation());
    return    model * glm::scale(identity, GetGlobalScale());
}

}

