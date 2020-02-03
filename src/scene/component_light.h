#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace pg
{

/////////////////////////////////////
class LightComponent
{
public:
    LightComponent();

    virtual glm::mat4       GetViewMatrix(glm::vec3 const& pos, glm::vec3 const& forward) const = 0;
    virtual glm::mat4       GetProjectionMatrix() const = 0;

private:
    glm::vec3   m_Color;
    float       m_Intensity;
    
};


/////////////////////////////////////
class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent();

    virtual glm::mat4       GetViewMatrix(glm::vec3 const& pos, glm::vec3 const& forward) const override;
    virtual glm::mat4       GetProjectionMatrix() const override;

private:
    glm::vec3   m_GlobalDirection;
    float       m_ViewWidth;
};


/////////////////////////////////////
class SpotLightComponent : public LightComponent
{
    SpotLightComponent();

    virtual glm::mat4       GetViewMatrix(glm::vec3 const& pos, glm::vec3 const& forward) const override;
    virtual glm::mat4       GetProjectionMatrix() const override;

private:
    glm::vec3   m_Direction;
    glm::vec3   m_MaxDistance;
    glm::vec3   m_FalloffDistance;
};

}

