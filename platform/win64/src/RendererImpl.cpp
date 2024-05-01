#include "platform/common/include/Renderer.h"

#define GLFW_INCLUDE_NONE
#include "platform/win64/thirdparty/glad/include/gl.h"
#include "platform/win64/thirdparty/glfw/include/glfw3.h"

#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <array>

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

class RendererImpl {

    std::shared_ptr<GLFWWindowWrapper> _window;
    std::shared_ptr<Platform> _platform { nullptr };

    GLuint vertex_array_object, vertex_buffer_object = 0;
    GLuint shader_program_object = 0;

	GLuint i_colour_location = 0;

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


        glGenBuffers(1, &vertex_buffer_object);

        
        glGenVertexArrays(1, &vertex_array_object);
        {
            glBindVertexArray(vertex_array_object);

            glEnableVertexAttribArray(0); // enable the first input variable (vertex position)
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        
        { // SHADERS

            const char* vertex_shader_str =
                "#version 410\n"
                "in vec3 a_position;\n"
                "void main() {\n"
                "gl_Position = vec4(a_position, 1.0);\n"
                "}";

            const char* fragment_shader_str =
                "#version 410\n"
                "out vec4 o_frag_colour;\n"
                "uniform vec4 i_colour;\n"
                "void main() {\n"
                "o_frag_colour = i_colour;\n"
                "}\n";

            GLuint vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader_object, 1, &vertex_shader_str, NULL);
            glCompileShader(vertex_shader_object);

            GLuint fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader_object, 1, &fragment_shader_str, NULL);
            glCompileShader(fragment_shader_object);

            shader_program_object = glCreateProgram();
            glBindAttribLocation(shader_program_object, 0, "a_position");

            glAttachShader(shader_program_object, vertex_shader_object);
            glAttachShader(shader_program_object, fragment_shader_object);
            glLinkProgram(shader_program_object);

            i_colour_location = glGetUniformLocation(shader_program_object, "i_colour");

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        }

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
        while (true) {}
    }

    inline void frameBegin()
    {
        const Size windowSize = getWindowSize();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, windowSize.width, windowSize.height);

    }

    inline void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept
    {
        const auto windowSize = getWindowSize();
        const std::array<float, 2> scaledTopLeft = { topLeft.x / static_cast<float>(windowSize.width), topLeft.y / static_cast<float>(windowSize.height) };
        const std::array<float, 2> scaledSize = { size.width / static_cast<float>(windowSize.width), size.height / static_cast<float>(windowSize.height) };

        std::array<float, 3 * 4> quadVerts =
        {
            scaledTopLeft[0] - 0.5, scaledTopLeft[1] - 0.5, 0.0f,
            scaledTopLeft[0] + scaledSize[0] - 0.5, scaledTopLeft[1] - 0.5, 0.0f,
            scaledTopLeft[0] - 0.5, scaledTopLeft[1] + scaledSize[1] - 0.5, 0.0f,
            scaledTopLeft[0] + scaledSize[0] - 0.5, scaledTopLeft[1] + scaledSize[1] - 0.5, 0.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, quadVerts.data(), GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

        glUseProgram(shader_program_object);
        {
            glUniform4f(i_colour_location, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
            glBindVertexArray(vertex_array_object);
            {
                //glDrawArrays(GL_TRIANGLES, 0, 3);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            glBindVertexArray(0);
        }
        glUseProgram(0);

        auto hasError = glGetError();
        if (hasError != GL_NO_ERROR) {
            std::cout << "GL Error: " << hasError << std::endl;
            while (true) {}

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
{
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