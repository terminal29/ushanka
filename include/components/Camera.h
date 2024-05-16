#pragma once
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace U {

    static constexpr glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

    class Renderer;

    class Camera {
        std::shared_ptr<Renderer> _renderer;

        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 _target = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 _direction{};
        glm::vec3 _right{};
        glm::vec3 _up{};

        glm::mat4 _viewMatrix{};

        glm::mat4 _projectionMatrix{};

        //

        void recalc() noexcept;

    public:
        Camera(std::shared_ptr<Renderer> renderer) noexcept;

        void setTarget(const glm::vec3& target) noexcept;
        void setPosition(const glm::vec3& position) noexcept;

        const glm::vec3& getPosition() const noexcept;
        const glm::vec3& getTarget() const noexcept;
        const glm::vec3& getDirection() const noexcept;
        const glm::mat4& getViewMatrix() const noexcept;
		const glm::mat4& getProjectionMatrix() const noexcept;
    };

}