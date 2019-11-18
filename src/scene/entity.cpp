#include "entity.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include <glm/ext/quaternion_common.hpp>

namespace pg
{

Entity::Entity()
    : m_Parent{ nullptr }
{}

Entity::Entity(Entity* parent, std::string const& name, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    : m_Name{ name }
    , m_Parent{ parent }
    , m_RelativePos{ position }
    , m_RelativeRotation{ rotation }
    , m_RelativeScale{ scale }
{

}

Entity* Entity::GetParent()
{
    return m_Parent;
}

Entity* Entity::GetChild(std::uint32_t child)
{
    return m_Children[child].get();
}

Entity const* Entity::GetChild(std::uint32_t child) const
{
    return m_Children[child].get();
}

Entity* Entity::FindChild(std::string const& name)
{
    auto result = std::find_if(
        m_Children.begin(),
        m_Children.end(),
        [&name](std::unique_ptr<Entity> const& entity)
        {
            return name == entity->GetName();
        });

    if (result == m_Children.end())
    {
        return nullptr;
    }
    else
    {
        return (*result).get();
    }
}

Entity* Entity::FindChildRecursive(std::string const& name)
{
    Entity* result = nullptr;

    std::uint32_t childCount = 0;
    for (std::uint32_t i = 0; i < childCount; i++)
    {
        if (name == m_Children[i]->GetName())
        {
            result = m_Children[i].get();
        }
    }

    if (!result)
    {
        for (std::uint32_t i = 0; i < childCount; i++)
        {
            result = m_Children[i]->FindChildRecursive(name);
        }
    }
    
    return result;
}

Entity* Entity::AddChild(std::string const& name, glm::vec3 const& pos, glm::quat const& rot, glm::vec3 const& scale)
{
    auto sameName = std::find_if(
        m_Children.begin(),
        m_Children.end(),
        [&name](std::unique_ptr<Entity> const& entity)
        {
            return name == entity->GetName();
        }
    );
    assert(sameName != m_Children.end() && "Entity can't have two children with same names");
    assert(!name.empty() && "Entity can't have an empty name");


    std::unique_ptr<Entity> entity{ new Entity{ this, name, pos, rot, scale } };
    Entity* ptr = entity.get();
    m_Children.emplace_back(std::move(entity));
    return ptr;
}

std::string const& Entity::GetName() const
{
    return m_Name;
}

glm::vec3 const& Entity::GetPosition() const
{
    return m_RelativePos;
}

glm::quat const& Entity::GetRotation() const
{
    return m_RelativeRotation;
}

glm::vec3 const& Entity::GetScale() const
{
    return m_RelativeScale;
}

void Entity::SetPosition(glm::vec3 const& pos)
{
    m_RelativePos = pos;
}

void Entity::SetRotation(glm::quat const& rot)
{
    m_RelativeRotation = rot;
}

void Entity::SetScale(glm::vec3 const& scale)
{
    m_RelativeScale = scale;
}

glm::vec3 Entity::GetGlobalPosition() const
{
    glm::vec3 result = m_RelativePos;
    return m_Parent ? result + m_Parent->GetGlobalPosition() : result;
}

glm::quat Entity::GetGlobalRotation() const
{
    glm::quat result = m_RelativeRotation;
    return m_Parent ? m_RelativeRotation * m_Parent->GetGlobalRotation() : m_RelativeRotation;
}

glm::vec3 Entity::GetGlobalScale() const
{
    glm::vec3 result = m_RelativeScale;
    return m_Parent ? m_RelativeScale * m_Parent->GetGlobalScale() : m_RelativeScale;
}

void Entity::SetGlobalPosition(glm::vec3 const& globalPos)
{
    glm::vec3 const currentPos  = GetGlobalPosition();
    glm::vec3 const diff        = globalPos - currentPos;

    SetPosition(GetPosition() + diff);
}

void Entity::SetGlobalRotation(glm::quat const& globalRotation)
{
    glm::quat const currentRot  = GetGlobalRotation();
    glm::quat const diff        = glm::inverse(currentRot) * globalRotation; 
    
    SetRotation(GetRotation() * diff);
}

void Entity::SetGlobalScale(glm::vec3 const& globalScale)
{
    glm::vec3 const currentScale    = GetGlobalScale();
    glm::vec3 const diff            = globalScale / currentScale;

    SetScale(GetScale() * diff);
}


} // namespace pg

