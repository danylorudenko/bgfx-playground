#pragma once

#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <gfx/gfx_settings.h>

namespace pg
{

struct View
{
    std::uint16_t x;
    std::uint16_t y;
    std::uint16_t width;
    std::uint16_t height;
};

enum class ProjectionType
{
    Perspective,
    Ortho
};

struct Projection
{
    ProjectionType  type;
    float           fovDegrees;
    float           nearPlane;
    float           farPlane;
};



class ViewProvider
{
public:
    ViewProvider(Projection projection, View view);

    ViewProvider(ViewProvider const& rhs);
    ViewProvider(ViewProvider&& rhs);

    ViewProvider& operator=(ViewProvider const& rhs);
    ViewProvider& operator=(ViewProvider&& rhs);

    ~ViewProvider();

    inline View&                GetView() { return m_View; }
    inline View const&          GetView() const { return m_View; }
    inline void                 SetView(View const& view) { m_View = view; }

    inline Projection&          GetProjection() { return m_Projection; }
    inline Projection const&    GetProjection() const { return m_Projection; }
    inline void                 SetProjection(Projection const& projection) { m_Projection = projection; }

    glm::mat4                   GetDefaultViewMatrix(glm::vec3 const& position, glm::vec3 const& forwardVector) const;
    glm::mat4                   GetDefaultProjectionMatrix() const;

private:
    Projection  m_Projection;
    View        m_View;

};

} // namespace pg

