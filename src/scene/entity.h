#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include <gfx/gfx_objects.h>

#include <scene/renderable_component.h>

namespace pg
{

class Entity
{
public:
    Entity();
    ~Entity() = default;

    // Hierarchy
    Entity*                             GetParent();
    Entity*                             GetChild(std::uint32_t child);
    Entity const*                       GetChild(std::uint32_t child) const;
    Entity*                             FindChild(std::string const& name);
    Entity*                             FindChildRecursive(std::string const& name);
    Entity*                             AddChild(
                                            std::string const& name, 
                                            glm::vec3 const& pos = glm::vec3{},
                                            glm::quat const& rot = glm::quat{},
                                            glm::vec3 const& scale = glm::vec3{});

    std::uint32_t                       GetChildCount() const { return static_cast<std::uint32_t>(m_Children.size()); }
    inline std::string const&           GetName() const;

// Coordinates
    inline glm::vec3 const&             GetPosition() const;
    inline glm::quat const&             GetRotation() const;
    inline glm::vec3 const&             GetScale() const;
    inline void                         SetPosition(glm::vec3 const& pos);
    inline void                         SetRotation(glm::quat const& rot);
    inline void                         SetScale(glm::vec3 const& scale);

    inline glm::vec3                    GetGlobalPosition() const;
    inline glm::quat                    GetGlobalRotation() const;
    inline glm::vec3                    GetGlobalScale() const;
    inline void                         SetGlobalPosition(glm::vec3 const& pos);
    inline void                         SetGlobalRotation(glm::quat const& rot);
    inline void                         SetGlobalScale(glm::vec3 const& scale);

// Graphics
    inline RenderableComponent&         GetRenderableComponent() { return m_RenderableComponent; }
    inline RenderableComponent const&   GetRenderableComponent() const { return m_RenderableComponent; }

    inline void                         SetVertexBuffer(std::shared_ptr<bgfx_vertex_buffer_handle_t> const& vertexBuffer) { m_RenderableComponent.m_VertexBuffer = vertexBuffer; }
    inline void                         SetShader(gfx::ShaderRef const& program) { m_RenderableComponent.m_ShaderProgram = program; }


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
// General info
    std::string m_Name;

    Entity* m_Parent;
    std::vector<std::unique_ptr<Entity>> m_Children;

    glm::vec3   m_RelativePos;
    glm::quat   m_RelativeRotation;
    glm::vec3   m_RelativeScale;

// Rendering
    RenderableComponent   m_RenderableComponent;
};

} // namespace pg

