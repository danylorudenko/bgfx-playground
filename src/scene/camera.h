#pragma once

#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/mat4x4.hpp>

#include <math/transformable.h>
#include <math/view_provider.h>

namespace pg
{

class Camera 
    : public Transformable
    , public ViewProvider
{
public:
    Camera();
    Camera(Camera const& rhs);
    Camera(Camera&& rhs);

    Camera& operator=(Camera const& rhs);
    Camera& operator=(Camera&& rhs);

    ~Camera();

    void                        Update(float dt);
};

} // namespace pg

