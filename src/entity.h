#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

class Entity
{
public:
    Entity() = default;
    //Entity(Entity* parent,
    //    std::string const& name,
    //    bx::Vec3 position = bx::Vec3{},
    //    bx::Quaternion rotation = bx::Quaternion{},
    //    bx::Vec3 scale = bx::Vec3{});

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
    //bx::Vec3        GetPosition();
    //bx::Quaternion  GetRotation();
    //bx::Vec3        GetScale();
    //void            SetPosition(bx::Vec3 pos);
    //void            SetRotation(bx::Quaternion rot);
    //void            SetScale(bx::Vec3 scale);
    //
    //bx::Vec3        GetGlobalPosition();
    //bx::Quaternion  GetGlobalRotation();
    //bx::Vec3        GetGlobalScale();
    //void            SetGlobalPosition(bx::Vec3 pos);
    //void            SetGlobalRotation(bx::Quaternion rot);
    //void            SetGlobalScale(bx::Vec3 scale);



private:
    std::string m_Name;
    std::vector<std::unique_ptr<Entity>> m_Children;

    //bx::Vec3        m_RelativePos;
    //bx::Quaternion  m_RelativeRotation;
    //bx::Vec3        m_RelativeScale;
};

