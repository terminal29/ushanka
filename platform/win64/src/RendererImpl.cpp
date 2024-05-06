#include "platform/common/include/Renderer.h"

#define GLFW_INCLUDE_NONE
#include "platform/win64/thirdparty/glad/include/gl.h"
#include "platform/win64/thirdparty/glfw/include/glfw3.h"

#include <array>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>


#include "platform/common/include/Shader.h"

std::function<void(int, const char*)> glErrorCallback;

void onOpenGLError(int error, const char* description)
{
    if (glErrorCallback) {
        glErrorCallback(error, description);
    }
}

class GLFWWindowWrapper {
    GLFWwindow* _window { nullptr };

public:
    inline GLFWWindowWrapper(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
    {
        _window = glfwCreateWindow(width, height, title, monitor, share);
    }
    inline GLFWwindow* get() noexcept
    {
        return _window;
    }
    ~GLFWWindowWrapper()
    {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
};

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    //if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

class RendererImpl {

    std::shared_ptr<GLFWWindowWrapper> _window;
    std::shared_ptr<Platform> _platform { nullptr };

    std::unique_ptr<Shader> _shader;

    const std::string vertexShader = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "layout (location = 1) in vec3 aColor;\n"
                                     "out vec3 ourColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "gl_Position = vec4(aPos, 1.0);\n"
                                     "ourColor = aColor;\n"
                                     "}\n";

    const std::string fragmentShader = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec3 ourColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "FragColor = vec4(ourColor, 1.0); \n"
                                       "}\n";

public:
    inline RendererImpl(std::shared_ptr<Platform> platform)
        : _platform(platform)
    {
        // init glfw
        if (!glfwInit()) {
            // todo: fail gracefully
            std::terminate();
        }
        // Setup error callback
        glErrorCallback = [&](int error, const char* description) {
            onOpenGLError(error, description);
        };
        glfwSetErrorCallback(::onOpenGLError);

        // make window
        _window = std::make_shared<GLFWWindowWrapper>(640, 480, "My Title", nullptr, nullptr);

        // setup context
        glfwMakeContextCurrent(_window->get());

        // load gl fns
        gladLoadGL(glfwGetProcAddress);

        // set vsync on
        glfwSwapInterval(1);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        _shader = std::make_unique<Shader>(vertexShader, fragmentShader);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline ~RendererImpl()
    {
        // ensure window closes before destructing glfw
        _window.reset();
        glfwTerminate();
    }

    inline void onOpenGLError(int error, const char* description)
    {
        std::cout << std::format("GL Error {} - {}", error, description) << std::endl;
        while (true) { }
    }

    inline void frameBegin()
    {
        const Size windowSize = getWindowSize();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, windowSize.width, windowSize.height);
        unsigned long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        float hue = (milliseconds_since_epoch / 100) % 360 / 360.0f;
        RGBColor squareColor = HSLToRGB(hue, 1.0, 0.5);
        glClearColor(squareColor.r / 255.f, squareColor.g / 255.f, squareColor.b / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
    {
        const auto windowSize = getWindowSize();
        const std::array<float, 2> scaledTopLeft = { topLeft.x / static_cast<float>(windowSize.width), topLeft.y / static_cast<float>(windowSize.height) };
        const std::array<float, 2> scaledSize = { size.width / static_cast<float>(windowSize.width), size.height / static_cast<float>(windowSize.height) };

        std::vector<Shader::vertex_t> quadVerts;
        quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] - 0.5f, 0.0f });
        quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] - 0.5f, 0.0f });
        quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, 0.0f });
        quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, 0.0f });

        _shader->drawVertices(quadVerts, color);

        auto hasError = glGetError();
        if (hasError != GL_NO_ERROR) {
            std::cout << "GL Error: " << hasError << std::endl;
            while (true) { }
        }
    }

    inline void frameEnd()
    {
        glfwSwapBuffers(_window->get());
    }

    inline bool wait() const noexcept
    {
        glfwPollEvents();
        return !glfwWindowShouldClose(_window->get());
    }

    inline Size getWindowSize() const noexcept
    {
        int width, height;
        glfwGetFramebufferSize(_window->get(), &width, &height);
        return Size { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }
};

void Renderer::RendererImplDeleter::operator()(RendererImpl* ptr)
{
    std::default_delete<RendererImpl>()(ptr);
}

Renderer::Renderer(std::shared_ptr<Platform> platform)
    : _platform(platform)
    , _pimpl(std::unique_ptr<RendererImpl, RendererImplDeleter>(new RendererImpl(platform)))
    , _activeCamera(std::make_shared<Camera>())
{
}

std::shared_ptr<Camera> Renderer::getActiveCamera() const noexcept
{
    return _activeCamera;
}

void Renderer::setActiveCamera(std::shared_ptr<Camera> camera) noexcept
{
    _activeCamera = camera;
}

const Size Renderer::getWindowSize() const noexcept
{
    return _pimpl->getWindowSize();
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