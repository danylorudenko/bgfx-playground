#include <math/view_provider.h>

#include <gfx/gfx_settings.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace pg
{

ViewProvider::ViewProvider(Projection projection, View view)
    : m_Projection{ projection }
    , m_View{ view }
{}

ViewProvider::ViewProvider(ViewProvider const& rhs) = default;

ViewProvider::ViewProvider(ViewProvider&& rhs) = default;

ViewProvider& ViewProvider::operator=(ViewProvider const& rhs) = default;

ViewProvider& ViewProvider::operator=(ViewProvider&& rhs) = default;

ViewProvider::~ViewProvider() = default;

glm::mat4 ViewProvider::GetDefaultViewMatrix(glm::vec3 const& position, glm::vec3 const& forwardVector) const
{
    glm::vec3 constexpr upVector{ 0.0f, 1.0f, 0.0f };
    return glm::lookAtLH(position, position + forwardVector, upVector);
}

glm::mat4 ViewProvider::GetDefaultProjectionMatrix() const
{
    return glm::perspectiveLH(m_Projection.fovDegrees, m_View.width / m_View.height, m_Projection.nearPlane, m_Projection.farPlane);
}

} // namespace pg

