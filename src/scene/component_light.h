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

    virtual glm::mat4       GetViewProjectionMatrix() const = 0;

private:
    glm::vec3   m_Color;
    float       m_Intensity;
    
};


/////////////////////////////////////
class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent();

    virtual glm::mat4       GetViewProjectionMatrix() const override;

private:
    glm::vec3   m_GlobalDirection;
    float       m_ViewWidth;
};


/////////////////////////////////////
class SpotLightComponent : public LightComponent
{
    SpotLightComponent();

    virtual glm::mat4       GetViewProjectionMatrix() const override;

private:
    glm::vec3   m_Direction;
    glm::vec3   m_MaxDistance;
    glm::vec3   m_FalloffDistance;
};

}

