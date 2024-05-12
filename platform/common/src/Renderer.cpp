#include <platform/Renderer.h>
#include <NativeRenderer.h>

U::Renderer::Renderer(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _impl(std::make_unique<NativeRenderer>(platform))
{
}

const U::Size U::Renderer::getWindowSize() const noexcept
{
    return _impl->getWindowSize();
}

//void U::Renderer::drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
//{
//    _impl->drawQuad(topLeft, size, color);
//}
//
//void U::Renderer::drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept
//{
//    _impl->drawVertices(camera, vertices, color);
//}

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

void U::Renderer::draw(const Camera& camera, const std::shared_ptr<U::Mesh> mesh, const std::shared_ptr<U::Shader> shader) noexcept
{
    _impl->draw(camera, mesh, shader);
}

U::Renderer::~Renderer() = default;