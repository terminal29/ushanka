#pragma once

#include "util/Color.h"
#include "util/Point.h"
#include "util/Size.h"
#include "platform/Platform.h"
#include "components/Camera.h"
#include "components/Transform.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <platform/Mesh.h>
#include <platform/Shader.h>

namespace U {

    class NativeRenderer;

    class Renderer {
        std::shared_ptr<Platform> _platform;

        std::unique_ptr<NativeRenderer> _impl;

    public:
        Renderer(std::shared_ptr<Platform> platform);
        ~Renderer();

        bool waitForVSync() const noexcept;

        const Size getWindowSize() const noexcept;

        void frameBegin() noexcept;

        void frameEnd() noexcept;

		void draw(const Camera& camera, const Transform& objectTransform, const std::shared_ptr<U::Mesh>, const std::shared_ptr<U::Shader> shader) noexcept;
    };

}