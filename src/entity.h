#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace pg
{

class Entity
{
public:
    Entity();
    ~Entity() = default;

    // Hierarchy
    Entity* GetParent();
    Entity* GetChild(std::uint32_t child);
    Entity* FindChild(std::string const& name);
    Entity* AddChild(
        std::string const& name, 
        glm::vec3 const& pos = glm::vec3{},
        glm::quat const& rot = glm::quat::{},
        glm::vec3 const& scale = glm::vec3{});

    inline std::string const&   GetName() const;

    // Coordinates
    inline glm::vec3 const&     GetPosition() const;
    inline glm::quat const&     GetRotation() const;
    inline glm::vec3 const&     GetScale() const;
    inline void                 SetPosition(glm::vec3 const& pos);
    inline void                 SetRotation(glm::quat const& rot);
    inline void                 SetScale(glm::vec3 const& scale);

    inline glm::vec3            GetGlobalPosition() const;
    inline glm::quat            GetGlobalRotation() const;
    inline glm::vec3            GetGlobalScale() const;
    inline void                 SetGlobalPosition(glm::vec3 const& pos);
    inline void                 SetGlobalRotation(glm::quat const& rot);
    inline void                 SetGlobalScale(glm::vec3 const& scale);


private:
    Entity(
        Entity* parent,
        std::string const& name,
        glm::vec3 position = glm::vec3{},
        glm::quat rotation = glm::quat{},
        glm::vec3 scale = glm::vec3{});

    Entity(Entity const&) = delete;
    Entity& operator=(Entity const&) = delete;

    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;


private:
    std::string m_Name;

    Entity* m_Parent;
    std::vector<std::unique_ptr<Entity>> m_Children;

    glm::vec3   m_RelativePos;
    glm::quat   m_RelativeRotation;
    glm::vec3   m_RelativeScale;
};

} // namespace pg

