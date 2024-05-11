#pragma once

#include "util/Color.h"
#include "util/Point.h"
#include "util/Size.h"
#include "platform/Platform.h"
#include "components/Camera.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>


namespace U {

    class RendererImpl;

    class Renderer {
        std::shared_ptr<Platform> _platform;

        struct RendererImplDeleter {
            void operator()(RendererImpl* p);
        };
        std::unique_ptr<RendererImpl, RendererImplDeleter> _pimpl;

    public:
        Renderer(std::shared_ptr<Platform> platform);

        bool wait() const noexcept;

        const Size getWindowSize() const noexcept;

        void frameBegin() noexcept;
        void frameEnd() noexcept;

        void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept;

		void drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept;
    };

}