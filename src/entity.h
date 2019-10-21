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
    Entity() = default;
    Entity(Entity* parent,
        std::string const& name,
        glm::vec3 position = glm::vec3{},
        glm::quat rotation = glm::quat{},
        glm::vec3 scale = glm::vec3{});

    Entity(Entity const&) = delete;
    Entity(Entity&&) = default;
    Entity& operator=(Entity const&) = delete;
    Entity& operator=(Entity&&) = default;

    ~Entity() = default;

    // Hierarchy
    Entity* GetChild(std::size_t child);
    Entity* FindChild(std::string const& name);
    void    SetParent(Entity* parent);
    void    AddChild(Entity* child);


    // Coordinates
    glm::vec3       GetPosition();
    glm::quat       GetRotation();
    glm::vec3       GetScale();
    void            SetPosition(glm::vec3 pos);
    void            SetRotation(glm::quat rot);
    void            SetScale(glm::vec3 scale);
    
    glm::vec3       GetGlobalPosition();
    glm::quat       GetGlobalRotation();
    glm::vec3       GetGlobalScale();
    void            SetGlobalPosition(glm::vec3 pos);
    void            SetGlobalRotation(glm::quat rot);
    void            SetGlobalScale(glm::vec3 scale);



private:
    std::string m_Name;
    std::vector<std::unique_ptr<Entity>> m_Children;

    glm::vec3   m_RelativePos;
    glm::quat   m_RelativeRotation;
    glm::vec3   m_RelativeScale;
};

} // namespace pg

