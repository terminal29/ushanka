#pragma once
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace U {

    inline constexpr glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

    class Renderer;

    class Camera {
        std::shared_ptr<Renderer> _renderer;

        glm::fvec3 _position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::fvec3 _target = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::fvec3 _direction{};
        glm::fvec3 _right{};
        glm::fvec3 _up{};

        glm::fmat4 _viewMatrix{1.0f};

        glm::fmat4 _projectionMatrix{1.0f};

        //

        void recalc() noexcept;

    public:
        Camera(std::shared_ptr<Renderer> renderer) noexcept;

        void setTarget(const glm::fvec3& target) noexcept;
        void setPosition(const glm::fvec3& position) noexcept;

        const glm::fvec3& getPosition() const noexcept;
        const glm::fvec3& getTarget() const noexcept;
        const glm::fvec3& getDirection() const noexcept;
        const glm::fmat4& getViewMatrix() const noexcept;
		const glm::fmat4& getProjectionMatrix() const noexcept;
    };

}