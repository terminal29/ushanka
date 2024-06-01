#include <platform/Renderer.h>
#include <NativeRenderer.h>

U::Renderer::Renderer(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _impl(std::make_unique<NativeRenderer>(platform))
{
}

U::Renderer::~Renderer() = default;

const U::Size U::Renderer::getWindowSize() const noexcept
{
    return _impl->getWindowSize();
}

bool U::Renderer::waitForVSync() const noexcept
{
    return _impl->waitForVSync();
}

void U::Renderer::frameBegin() noexcept
{
    _impl->frameBegin();
}

void U::Renderer::frameEnd() noexcept
{
    _impl->frameEnd();
}

void U::Renderer::drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept
{
	_impl->drawVoxels(camera, globalOffset, voxels);
}

void U::Renderer::setClearColor(const RGBColor& color) noexcept
{
	_impl->setClearColor(color);
}