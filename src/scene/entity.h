#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/matrix_float4x4.hpp>

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
                                            glm::vec3 const& pos = glm::vec3{ 0.0f, 0.0f, 0.0f },
                                            glm::quat const& rot = glm::identity<glm::quat>(),
                                            glm::vec3 const& scale = glm::vec3{ 1.0f, 1.0f, 1.0f });

    std::uint32_t                       GetChildCount() const { return static_cast<std::uint32_t>(m_Children.size()); }
    inline std::string const&           GetName() const { return m_Name; }
    inline void                         SetName(std::string const& name) { m_Name = name; }

// Coordinates
    glm::vec3 const&                    GetPosition() const;
    glm::quat const&                    GetRotation() const;
    glm::vec3 const&                    GetScale() const;
    void                                SetPosition(glm::vec3 const& pos);
    void                                SetRotation(glm::quat const& rot);
    void                                SetScale(glm::vec3 const& scale);

    glm::vec3                           GetGlobalPosition() const;
    glm::quat                           GetGlobalRotation() const;
    glm::vec3                           GetGlobalScale() const;
    void                                SetGlobalPosition(glm::vec3 const& pos);
    void                                SetGlobalRotation(glm::quat const& rot);
    void                                SetGlobalScale(glm::vec3 const& scale);

// Graphics
    glm::mat4                           GetGlobalModelMatrix() const;

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

    Entity* m_Parent;
    std::vector<std::unique_ptr<Entity>> m_Children;

    glm::vec3   m_RelativePos;
    glm::quat   m_RelativeRotation;
    glm::vec3   m_RelativeScale;

// Rendering
    std::unique_ptr<RenderableComponent>   m_RenderableComponent;
};

} // namespace pg

