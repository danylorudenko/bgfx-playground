#include "entity.h"

#include <algorithm>
#include <cassert>
#include <utility>

namespace pg
{

Entity::Entity() = default;

Entity::Entity(Entity* parent, std::string const& name, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    : Transformable{ parent, position, rotation, scale }
    , m_Name{ name }
{
}

Entity* Entity::GetParent()
{
    return reinterpret_cast<Entity*>(GetTransformableParent());
}

Entity const* Entity::GetParent() const
{
    return reinterpret_cast<Entity const*>(GetTransformableParent());
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

    assert(!name.empty() && "Entity can't have an empty name");
    assert(m_Children.size() == 0 || (sameName == m_Children.end()) && "Entity can't have two children with same names");


    std::unique_ptr<Entity> entity{ new Entity{ this, name, pos, rot, scale } };
    Entity* ptr = entity.get();
    m_Children.emplace_back(std::move(entity));
    return ptr;
}

void Entity::InitRenderableComponent(gfx::SharedShaderProgram const& shaderRef, gfx::SharedVertexBuffer const& vertexBuffer)
{
    m_RenderableComponent = std::make_unique<RenderableComponent>(shaderRef, vertexBuffer);
}

bool Entity::IsRenderable() const
{
    return static_cast<bool>(m_RenderableComponent);
}


} // namespace pg

