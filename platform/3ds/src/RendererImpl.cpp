#include "platform/common/include/Renderer.h"
#include "platform/common/include/Shader.h"
#include <3ds.h>
#include <chrono>
#include <citro2d.h>
#include <functional>
#include <iostream>
#include <memory>

constexpr Size ThreeDSWindowSize { 400, 240 };

constexpr u32 COMMON_DISPLAY_TRANSFER_FLAGS = (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

u32 _createRGBA(int r, int g, int b, int a)
{
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        + (a & 0xff);
}

class RendererImpl {

    std::shared_ptr<C3D_RenderTarget> _rtTopLeft { nullptr };
    std::shared_ptr<Platform> _platform { nullptr };
    std::unique_ptr<Shader> _shader { nullptr };

public:
    inline RendererImpl(std::shared_ptr<Platform> platform)
        : _platform(platform)
    {
        gfxInitDefault();
        consoleInit(GFX_BOTTOM, NULL);
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        _rtTopLeft = std::shared_ptr<C3D_RenderTarget>(C3D_RenderTargetCreate(ThreeDSWindowSize.height, ThreeDSWindowSize.width, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8), [](C3D_RenderTarget* target) {
            if (target != nullptr)
                C3D_RenderTargetDelete(target);
        });
        // cornflower blue
        C3D_RenderTargetClear(_rtTopLeft.get(), C3D_CLEAR_ALL, 0x68B0D8FF, 0);
        C3D_RenderTargetSetOutput(_rtTopLeft.get(), GFX_TOP, GFX_LEFT, COMMON_DISPLAY_TRANSFER_FLAGS);
        // not implemented to compile at runtime, maybe in future :)
        _shader = std::make_unique<Shader>("", "");
    }
    inline void frameBegin() noexcept
    {
        unsigned long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        float hue = (milliseconds_since_epoch / 100) % 360 / 360.0f;
        auto color = HSLToRGB(hue, 1.0, 0.5);
        u32 colorAsInt = _createRGBA(color.r, color.g, color.b, 255);
        C3D_RenderTargetClear(_rtTopLeft.get(), C3D_CLEAR_ALL, colorAsInt, 0);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C3D_FrameDrawOn(_rtTopLeft.get());
    }

    inline void frameEnd() noexcept
    {
        printf("\x1b[1;1HUshanka game by jaocb1");
        printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
        printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
        printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);
        std::cout.flush();
        std::cout << "\x1B[H";
        C3D_FrameEnd(0);
    }

    inline void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
    {
        const auto windowSize = getWindowSize();
        // width/height switched because screen is like that
        const std::array<float, 2> scaledTopLeft = { topLeft.y / static_cast<float>(windowSize.height), topLeft.x / static_cast<float>(windowSize.width) };
        const std::array<float, 2> scaledSize = { size.height / static_cast<float>(windowSize.height), size.width / static_cast<float>(windowSize.width) };

        std::vector<Shader::vertex_t> quadVerts;
        quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] - 0.5f, -1.0f });
        quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] - 0.5f, -1.0f });
        quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, -1.0f });
        quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, -1.0f });

        _shader->drawVertices(quadVerts, color);
    }

    inline bool wait() const noexcept
    {
        return aptMainLoop();
    }

    inline Size getWindowSize() const noexcept
    {
        return ThreeDSWindowSize;
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