#pragma once

#include <glm/vec3.hpp>

namespace pg
{

/////////////////////////////////////
struct LightComponent
{
    LightComponent()
        : m_Color{ 1.0f, 1.0f, 1.0f }
        , m_Intensity{ 1.0f }
    {}

    glm::vec3   m_Color;
    float       m_Intensity;
    
};


/////////////////////////////////////
struct DirectionalLightComponent : public LightComponent
{
    DirectionalLightComponent()
        : LightComponent{}
        , m_GlobalDirection{ 0.0f, 1.0f, 0.0f }
    {}

    glm::vec3   m_GlobalDirection;
};


/////////////////////////////////////
struct SpotLightComponent : public LightComponent
{
    SpotLightComponent()
        : LightComponent{}
        , m_MaxDistance{ 50.0f }
        , m_FalloffDistance{ 25.0f }
    {}

    glm::vec3   m_Direction;
    glm::vec3   m_MaxDistance;
    glm::vec3   m_FalloffDistance;
};

}

