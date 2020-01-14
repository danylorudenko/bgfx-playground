#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace pg::gfx
{

struct SimpleVertex
{
    SimpleVertex(float x, float y, float z, float r, float g, float b)
        : m_Pos{ x, y, z }
        , m_Color{ r, g, b }
    {}

    SimpleVertex(glm::vec3 const& pos, glm::vec3 const& color)
        : m_Pos{ pos.x, pos.y, pos.z }
        , m_Color{ color.x, color.y, color.z }
    {}

    float m_Pos     [3];
    float m_Color   [3];
};

struct Vertex
{
    Vertex(float x, float y, float z, float nX, float nY, float nZ, float tX, float tY, float tZ, float btX, float btY, float btZ, float u, float v)
        : m_Pos{ x, y, z }
        , m_Normal{ nX, nY, nZ }
        , m_Tangent{ tX, tY, tZ }
        , m_Bitangent{ btX, btY, btZ }
        , m_UV{ u, v }
    {}

    Vertex(glm::vec3 const& pos, glm::vec3 const& normal, glm::vec3 const& tangent, glm::vec3 const& bitangent, glm::vec2 const& uv)
        : m_Pos{ pos.x, pos.y, pos.z }
        , m_Normal{ normal.x, normal.y, normal.z }
        , m_Tangent{ tangent.x, tangent.y, tangent.z }
        , m_Bitangent{ bitangent.x, bitangent.y, bitangent.z }
        , m_UV{ uv.x, uv.y }
    {}

    float m_Pos      [3];
    float m_Normal   [3];
    float m_Tangent  [3];
    float m_Bitangent[3];
    float m_UV       [2];
};

} // namespace pg::gfx

