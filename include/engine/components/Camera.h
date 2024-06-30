#pragma once
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace U::Engine::Render {
    class Renderer;
};

namespace U::Engine::Components {

    class Camera {
        std::shared_ptr<U::Engine::Render::Renderer> _renderer;

        glm::fvec3 _position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::fvec3 _target = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::fvec3 _direction{};
        glm::fvec3 _right{};
        glm::fvec3 _up{};

        glm::fmat4 _viewMatrix{1.0f};

        void recalc() noexcept;

    public:

        static constexpr glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);


        Camera(std::shared_ptr<U::Engine::Render::Renderer> renderer) noexcept;

        void setTarget(const glm::fvec3& target) noexcept;
        void setPosition(const glm::fvec3& position) noexcept;

        inline static constexpr float fov = 80.0f;
        inline static constexpr float nearPlane = 0.1f;
        inline static constexpr float farPlane = 1000.0f;

        const glm::fvec3& getPosition() const noexcept;
        const glm::fvec3& getTarget() const noexcept;
        const glm::fvec3& getDirection() const noexcept;
        const glm::fmat4& getViewMatrix() const noexcept;
    };

}