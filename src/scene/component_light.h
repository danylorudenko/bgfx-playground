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

    virtual glm::mat4       GetViewMatrix() = 0;

private:
    glm::vec3   m_Color;
    float       m_Intensity;
    
};


/////////////////////////////////////
class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent();

private:
    glm::vec3   m_GlobalDirection;
};


/////////////////////////////////////
class SpotLightComponent : public LightComponent
{
    SpotLightComponent();

private:
    glm::vec3   m_Direction;
    glm::vec3   m_MaxDistance;
    glm::vec3   m_FalloffDistance;
};

}

