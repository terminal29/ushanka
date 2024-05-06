#include "platform/common/include/Renderer.h"
#include <3ds.h>
#include <citro2d.h>
#include <functional>
#include <iostream>
#include <memory>

constexpr Size ThreeDSWindowSize { 400, 240 };

class RendererImpl {

    std::shared_ptr<C3D_RenderTarget> _rtTopLeft { nullptr };
    std::shared_ptr<Platform> _platform { nullptr };

public:
    inline RendererImpl(std::shared_ptr<Platform> platform)
        : _platform(platform)
    {
        gfxInitDefault();
        consoleInit(GFX_BOTTOM, NULL);
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        _rtTopLeft = std::shared_ptr<C3D_RenderTarget>(C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT), [](C3D_RenderTarget* target) {
            if (target != nullptr)
                C3D_RenderTargetDelete(target);
        });
    }
    inline void frameBegin() noexcept
    {
        u32 clearColor = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
        // Todo: dont do this
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(_rtTopLeft.get(), clearColor);
        C2D_SceneBegin(_rtTopLeft.get());
    }

    inline void frameEnd() noexcept
    {
        printf("\x1b[1;1HSimple citro2d shapes example");
        printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
        printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
        printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);

        C3D_FrameEnd(0);
    }

    inline void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
    {
    }

    inline bool wait() const noexcept
    {
        return aptMainLoop();
    }
};

void Renderer::RendererImplDeleter::operator()(RendererImpl* ptr)
{
    std::default_delete<RendererImpl>()(ptr);
}

Renderer::Renderer(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _pimpl(std::unique_ptr<RendererImpl, RendererImplDeleter>(new RendererImpl(platform)))
{
}

std::shared_ptr<Camera> Renderer::getActiveCamera() const noexcept
{
    return _activeCamera;
}

const Size Renderer::getWindowSize() const noexcept
{
    return ThreeDSWindowSize;
}

void Renderer::drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
{
    _pimpl->drawQuad(topLeft, size, color);
}

bool Renderer::wait() const noexcept
{
    return _pimpl->wait();
}

void Renderer::frameBegin() noexcept
{
    _pimpl->frameBegin();
}

void Renderer::frameEnd() noexcept
{
    _pimpl->frameEnd();
}