#include <NativeRenderer.h>
#include <util/Size.h>
#include <NativeMesh.h>

std::function<void(int, const char*)> glErrorCallback;

void onOpenGLError(int error, const char* description)
{
    if (glErrorCallback) {
        glErrorCallback(error, description);
    }
}

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

U::NativeRenderer::NativeRenderer(std::shared_ptr<U::Platform> platform)
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
    _window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(640, 480, app_title, nullptr, nullptr), [](GLFWwindow* ptr) {
        glfwDestroyWindow(ptr);
    });

    // setup context
    glfwMakeContextCurrent(_window.get());

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

U::NativeRenderer::~NativeRenderer()
{
    // ensure window closes before destructing glfw
    _window.reset();
    glfwTerminate();
}

void U::NativeRenderer::onOpenGLError(int error, const char* description)
{
    std::cout << std::format("GL Error {} - {}", error, description) << std::endl;
    while (true) { }
}

void U::NativeRenderer::frameBegin()
{
    const U::Size windowSize = getWindowSize();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowSize.width, windowSize.height);
    unsigned long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    float hue = (milliseconds_since_epoch / 100) % 360 / 360.0f;
    U::RGBColor squareColor = U::HSLToRGB(hue, 1.0, 0.5);
    glClearColor(squareColor.r / 255.f, squareColor.g / 255.f, squareColor.b / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void U::NativeRenderer::drawQuad(const U::Point& topLeft, const U::Size& size, const U::RGBColor& color) noexcept
{
    const auto windowSize = getWindowSize();
    const std::array<float, 2> scaledTopLeft = { topLeft.x / static_cast<float>(windowSize.width), topLeft.y / static_cast<float>(windowSize.height) };
    const std::array<float, 2> scaledSize = { size.width / static_cast<float>(windowSize.width), size.height / static_cast<float>(windowSize.height) };

    std::vector<vertex_t> quadVerts;
    quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] - 0.5f, 0.0f });
    quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] - 0.5f, 0.0f });
    quadVerts.push_back({ scaledTopLeft[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, 0.0f });
    quadVerts.push_back({ scaledTopLeft[0] + scaledSize[0] - 0.5f, scaledTopLeft[1] + scaledSize[1] - 0.5f, 0.0f });

    //_shader->drawVertices(quadVerts, color);

    auto hasError = glGetError();
    if (hasError != GL_NO_ERROR) {
        std::cout << "GL Error: " << hasError << std::endl;
        while (true) { }
    }
}

void U::NativeRenderer::draw(const Camera& camera, const std::shared_ptr<U::Mesh> mesh, const std::shared_ptr<U::Shader> shader) noexcept
{
    shader->bind();
    auto nativeMesh = mesh->getNativeMesh();
    glBindVertexArray(nativeMesh->_vao);
	glDrawArrays(GL_TRIANGLES, 0, nativeMesh->_numVertices);
    glBindVertexArray(0);
    shader->unbind();
}

void U::NativeRenderer::drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept
{
	std::vector<vertex_t> vertexData;
	for (const auto& vertex : vertices) {
		vertexData.push_back({ vertex.x, vertex.y, vertex.z });
	}
}

void U::NativeRenderer::frameEnd()
{
    glfwSwapBuffers(_window.get());
}

bool U::NativeRenderer::waitForVSync() const noexcept
{
    glfwPollEvents();
    return !glfwWindowShouldClose(_window.get());
}

U::Size U::NativeRenderer::getWindowSize() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(_window.get(), &width, &height);
    return Size { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}