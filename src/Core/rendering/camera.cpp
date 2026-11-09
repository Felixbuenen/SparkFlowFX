#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sparkflow::render {

    glm::mat4 Camera::view() const
    {
        return glm::lookAt(m_position, m_target, m_up);
    }

    glm::mat4 Camera::projection() const
    {
        return glm::perspective(m_verticalFovRadians, m_aspectRatio, m_nearPlane, m_farPlane);
    }

}