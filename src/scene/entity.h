#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <gfx/gfx_objects.h>

#include <math/transformable.h>

#include <scene/component_renderable.h>

namespace pg
{

class Entity : public Transformable
{
public:
    Entity();
    ~Entity() = default;

    // Hierarchy
    Entity*                             GetParent();
    Entity const*                       GetParent() const;
    Entity*                             GetChild(std::uint32_t child);
    Entity const*                       GetChild(std::uint32_t child) const;
    Entity*                             FindChild(std::string const& name);
    Entity*                             FindChildRecursive(std::string const& name);
    Entity*                             AddChild(
                                            std::string const& name, 
                                            glm::vec3 const& pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
                                            glm::quat const& rot = glm::identity<glm::quat>(),
                                            glm::vec3 const& scale = glm::vec3{ 1.0f, 1.0f, 1.0f });

    std::uint32_t                       GetChildCount() const { return static_cast<std::uint32_t>(m_Children.size()); }
    inline std::string const&           GetName() const { return m_Name; }
    inline void                         SetName(std::string const& name) { m_Name = name; }

// Graphics
    void                                InitRenderableComponent(gfx::SharedShaderProgram const& shader, gfx::SharedVertexBuffer const& vertexBuffer);
    bool                                IsRenderable() const;

    inline RenderableComponent&         GetRenderableComponentRef() { return *m_RenderableComponent; }
    inline RenderableComponent const&   GetRenderableComponentRef() const { return *m_RenderableComponent; }

    inline void                         SetVertexBuffer(gfx::SharedVertexBuffer const& vertexBuffer) { m_RenderableComponent->m_VertexBuffer = vertexBuffer; }
    inline void                         SetShader(gfx::SharedShaderProgram const& program) { m_RenderableComponent->m_ShaderProgram = program; }


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
    std::vector<std::unique_ptr<Entity>> m_Children;

// Rendering
    std::unique_ptr<RenderableComponent>   m_RenderableComponent;
};

} // namespace pg

