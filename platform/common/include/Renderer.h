#pragma once

#include "include/util/Color.h"
#include "include/util/Point.h"
#include "include/util/Size.h"
#include "platform/common/include/Platform.h"
#include <platform/common/include/Camera.h>
#include <memory>

class RendererImpl;

class Renderer {
    std::shared_ptr<Platform> _platform;
    std::shared_ptr<Camera> _activeCamera;

    struct RendererImplDeleter {
        void operator()(RendererImpl* p);
    };
    std::unique_ptr<RendererImpl, RendererImplDeleter> _pimpl;

public:
    Renderer(std::shared_ptr<Platform> platform);

    bool wait() const noexcept;

    const Size getWindowSize() const noexcept;

	void setActiveCamera(std::shared_ptr<Camera> camera) noexcept;
	std::shared_ptr<Camera> getActiveCamera() const noexcept;

    void frameBegin() noexcept;
    void frameEnd() noexcept;

    void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept;
};