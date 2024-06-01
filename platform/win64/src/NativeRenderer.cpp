#include <NativeRenderer.h>
#include <util/Size.h>
#include "game/registry/ShaderRegistry.h"
#include <print>

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
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

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
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);

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

//void U::NativeRenderer::draw(const Camera& camera, const Transform& objectTransform, const std::shared_ptr<U::Mesh> mesh, const std::shared_ptr<U::Shader> shader) noexcept
//{
//    shader->bind();
//    // set shader uniforms
//
//    //todo check these
//    auto model = glm::mat4();
//
//    model = glm::translate(model, objectTransform.position);
//    model = glm::scale(model, objectTransform.scale);
//    //ModelMatrix = glm::rotate(ModelMatrix, rotAngle, Rotation);
//
//
//
//
//	const auto view = camera.getViewMatrix();
//	const auto projection = camera.getProjectionMatrix();
//	shader->setUniform(NamedShaderUniform::ModelMatrix, model);
//	shader->setUniform(NamedShaderUniform::ViewMatrix, view);
//	shader->setUniform(NamedShaderUniform::ProjectionMatrix, projection);
//    shader->setUniform(NamedShaderUniform::AmbientLightColor, glm::vec3(0.5,0.5,0.5));
//    shader->setUniform(NamedShaderUniform::SunDirectionVector, glm::vec3(0, 1, 0));
//    shader->setUniform(NamedShaderUniform::SunColor, glm::vec3(0.5, 0.5, 0.5));
//
//
//
//    auto nativeMesh = mesh->getNativeMesh();
//    glBindVertexArray(nativeMesh->_vao);
//	glDrawArrays(GL_TRIANGLES, 0, nativeMesh->_numVertices);
//    glBindVertexArray(0);
//    shader->unbind();
//}

//void U::NativeRenderer::drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept
//{
//	std::vector<vertex_t> vertexData;
//	for (const auto& vertex : vertices) {
//		vertexData.push_back({ vertex.x, vertex.y, vertex.z });
//	}
//}




std::tuple<GLint, GLint, std::size_t> U::NativeRenderer::makeVoxelVaoVbo(const std::vector<U::Voxel>& voxels) {
    std::vector<vertex_t> vertices = meshVoxels(voxels);

    std::vector<U::vertex_element_t> vertex_elements_flat;
    vertex_elements_flat.reserve(vertices.size() * U::elements_per_vertex);
    for (auto& vertex : vertices)
    {
        for (auto& element : vertex)
        {
            vertex_elements_flat.push_back(element);
        }
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_elements_flat.size() * sizeof(decltype(vertex_elements_flat)::value_type), vertex_elements_flat.data(), GL_STATIC_DRAW);

    std::size_t offset = 0;
    constexpr std::size_t vertex_stride = 3;
    constexpr std::size_t normal_stride = 3;
    constexpr std::size_t texture_coord_stride = 2;
    constexpr std::size_t color_stride = 3;
    constexpr std::size_t stride = (vertex_stride + normal_stride + texture_coord_stride + color_stride) * sizeof(vertex_element_t);


    // vertex positions
    glEnableVertexAttribArray(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexPosition));
    glVertexAttribPointer(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexPosition), 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (vertex_stride * sizeof(vertex_element_t));

    // vertex normals
    glEnableVertexAttribArray(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexNormal));
    glVertexAttribPointer(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexNormal), 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (normal_stride * sizeof(vertex_element_t));

    // tex coord
    glEnableVertexAttribArray(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexTextureCoord));
    glVertexAttribPointer(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexTextureCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (texture_coord_stride * sizeof(vertex_element_t));

    // vertex color
    glEnableVertexAttribArray(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexColor));
    glVertexAttribPointer(_vaoParamPositions.at(Shader::ENamedVAOParameter::VertexColor), 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (color_stride * sizeof(vertex_element_t));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return std::make_tuple(VAO, VBO, vertex_elements_flat.size());

}

void fakeShader(const glm::fmat4& model, const glm::fmat4& view, const glm::fmat4& projection, const glm::vec3& vertex) {

	auto mT = glm::transpose(model);
	auto vT = glm::transpose(view);
	auto pT = glm::transpose(projection);
    auto expectedVertex0Position = vertex;
    glm::fvec4 r0{ vertex.x, vertex.y, vertex.z, 1.0f };
    glm::fvec4 r1;
    r1.x = glm::dot(mT[0], r0);
	r1.y = glm::dot(mT[1], r0);
	r1.z = glm::dot(mT[2], r0);
	r1.w = glm::dot(mT[3], r0);

	glm::fvec4 r2;
	r2.x = glm::dot(vT[0], r1);
	r2.y = glm::dot(vT[1], r1);
	r2.z = glm::dot(vT[2], r1);
	r2.w = glm::dot(vT[3], r1);

	glm::fvec4 r3;
	r3.x = glm::dot(pT[0], r2);
	r3.y = glm::dot(pT[1], r2);
	r3.z = glm::dot(pT[2], r2);
	r3.w = glm::dot(pT[3], r2);

	expectedVertex0Position = glm::vec3(r3.x, r3.y, r3.z);


	std::cout << "3ds shader expected {\n " << expectedVertex0Position.x << ",\n " << expectedVertex0Position.y << ",\n " << expectedVertex0Position.z << "\n}\n";
}

void U::NativeRenderer::drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept
{
	if (voxels.empty())
    {
        std::print("Voxels was empty!\n");
		return;
	}
    auto shader = ShaderRegistry::Registry.at(voxels[0].shaderID);
	if (!shader)
	{
        std::print("Shader not found\n");
		return;
	}
	shader->bind();

	//todo check these
	const glm::fmat4 model = glm::translate(glm::fmat4(1), glm::vec3(globalOffset));

    // set shader uniforms
	const glm::fmat4 view = camera.getViewMatrix();
	const glm::fmat4 projection = camera.getProjectionMatrix();
	shader->setUniformMat4(Shader::ENamedShaderUniform::ModelMatrix, model);
	shader->setUniformMat4(Shader::ENamedShaderUniform::ViewMatrix, view);
	shader->setUniformMat4(Shader::ENamedShaderUniform::ProjectionMatrix, projection);
	shader->setUniformMat3(Shader::ENamedShaderUniform::NormalMatrix, glm::fmat3(glm::transpose(glm::inverse(view * model))));
	shader->setUniformVec3(Shader::ENamedShaderUniform::AmbientLightColor, glm::fvec3(0.5, 0.5, 0.5));
	shader->setUniformVec3(Shader::ENamedShaderUniform::SunDirection, glm::fvec3(0, 1, 0));
	shader->setUniformVec3(Shader::ENamedShaderUniform::SunColor, glm::fvec3(0.5, 0.5, 0.5));


	auto expectedVertex0Position = glm::vec3(0.5f, 0.5f, 0.5f);
	expectedVertex0Position = projection * model * view * glm::vec4(expectedVertex0Position, 1.0f);
	std::cout << "expected at {\n " << expectedVertex0Position.x << ",\n " << expectedVertex0Position.y << ",\n " << expectedVertex0Position.z << "\n}\n";

    fakeShader(model, view, projection, glm::vec3(0.5f, 0.5f, 0.5f));




    // generate mesh for voxel(s)
    auto [vao, vbo, vertexCount] = makeVoxelVaoVbo(voxels);

    std::print("Rendering {} vertices (from {} voxels)\n", vertexCount / elements_per_vertex, voxels.size());
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
    
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