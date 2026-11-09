#pragma once

#include <numbers>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sfx::rendering {

    class Camera
    {
    public:
        void setAspectRatio(float aspectRatio) noexcept
        {
            m_aspectRatio = aspectRatio;
        }

        glm::mat4 view() const noexcept;
        glm::mat4 projection() const noexcept;

        glm::mat4 viewProjection() const noexcept
        {
            return projection() * view();
        }

    private:
        glm::vec3 m_position{ 0.0f, 1.5f, 6.0f };
        glm::vec3 m_target{ 0.0f, 0.5f, 0.0f };
        glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };

        float m_verticalFovRadians{ std::numbers::pi_v<float> / 4.0f };
        float m_nearPlane{ 0.1f };
        float m_farPlane{ 100.0f };
        float m_aspectRatio{ 1.0f };
    };

} // namespace sfx::rendering