#include <NativeRenderer.h>
#include <util/Size.h>
#include <NativeMesh.h>
#include "game/registry/ShaderRegistry.h"

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

// Function to add a face to the vertex data
void addFace(std::vector<U::vertex_t>& vertexData, const std::array<float, 3>& pos, const std::array<float, 3>& normal) {
    U::vertex_t vertex;

    // Position (3 floats)
    vertex[0] = pos[0];
    vertex[1] = pos[1];
    vertex[2] = pos[2];

    // Normal (3 floats)
    vertex[3] = normal[0];
    vertex[4] = normal[1];
    vertex[5] = normal[2];

    // Texture UV (2 floats)
    vertex[6] = 0.0f; // Example UV coordinate, you might want to calculate or set accordingly
    vertex[7] = 0.0f; // Example UV coordinate, you might want to calculate or set accordingly

    // Color (3 floats, normalized)
    vertex[8] = 1.0f; // Red
    vertex[9] = 1.0f; // Green
    vertex[10] = 1.0f; // Blue

    // Add to vertex data
    vertexData.push_back(vertex);
}

std::vector<U::vertex_t> meshVoxels(const std::vector<U::Voxel>& voxels) {
    std::vector<U::vertex_t> vertices;

    // Cube vertices relative to the center of the voxel
    const float halfSize = 0.5f;

    // Define normals for each face
    std::array<std::array<float, 3>, 6> faceNormals = {
        std::array<float, 3>{1.0f, 0.0f, 0.0f},  // Right
        std::array<float, 3>{-1.0f, 0.0f, 0.0f}, // Left
        std::array<float, 3>{0.0f, 1.0f, 0.0f},  // Top
        std::array<float, 3>{0.0f, -1.0f, 0.0f}, // Bottom
        std::array<float, 3>{0.0f, 0.0f, 1.0f},  // Front
        std::array<float, 3>{0.0f, 0.0f, -1.0f}  // Back
    };

    // Define the 8 vertices of a cube
    std::array<std::array<float, 3>, 8> cubeVertices = {
        std::array<float, 3>{-halfSize, -halfSize, -halfSize},
        std::array<float, 3>{halfSize, -halfSize, -halfSize},
        std::array<float, 3>{halfSize, halfSize, -halfSize},
        std::array<float, 3>{-halfSize, halfSize, -halfSize},
        std::array<float, 3>{-halfSize, -halfSize, halfSize},
        std::array<float, 3>{halfSize, -halfSize, halfSize},
        std::array<float, 3>{halfSize, halfSize, halfSize},
        std::array<float, 3>{-halfSize, halfSize, halfSize}
    };

    // Define the 6 faces of the cube, each face has 4 vertices (two triangles)
    std::array<std::array<int, 4>, 6> faceIndices = {
        std::array<int, 4>{0, 1, 5, 4}, // Right
        std::array<int, 4>{2, 3, 7, 6}, // Left
        std::array<int, 4>{3, 0, 4, 7}, // Top
        std::array<int, 4>{1, 2, 6, 5}, // Bottom
        std::array<int, 4>{4, 5, 6, 7}, // Front
        std::array<int, 4>{3, 2, 1, 0}  // Back
    };

    for (const auto& voxel : voxels) {
        for (int i = 0; i < 6; ++i) {
            std::array<float, 3> normal = faceNormals[i];

            for (int j = 0; j < 4; ++j) {
                std::array<float, 3> vertexPos = {
                    voxel.position[0] + cubeVertices[faceIndices[i][j]][0],
                    voxel.position[1] + cubeVertices[faceIndices[i][j]][1],
                    voxel.position[2] + cubeVertices[faceIndices[i][j]][2]
                };
                addFace(vertices, vertexPos, normal);
            }
        }
    }
    return vertices;
}

std::pair<GLint, std::size_t> U::NativeRenderer::makeVoxelVao(const std::vector<U::Voxel>& voxels) {
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

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_elements_flat.size() * sizeof(decltype(vertex_elements_flat)::value_type), vertex_elements_flat.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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
    return std::make_pair(VAO, vertex_elements_flat.size());

}

void U::NativeRenderer::drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept
{
	if (voxels.empty())
    {
		return;
	}
    auto shader = ShaderRegistry.at(voxels[0].shaderID);
	if (!shader)
	{
		std::cout << "Shader not found" << std::endl;
		return;
	}
	shader->bind();

	//todo check these
	auto model = glm::mat4();
    model = glm::translate(model, glm::vec3(globalOffset));

    // set shader uniforms
	const auto view = camera.getViewMatrix();
	const auto projection = camera.getProjectionMatrix();
	shader->setUniform(Shader::ENamedShaderUniform::ModelMatrix, model);
	shader->setUniform(Shader::ENamedShaderUniform::ViewMatrix, view);
	shader->setUniform(Shader::ENamedShaderUniform::ProjectionMatrix, projection);
	shader->setUniform(Shader::ENamedShaderUniform::AmbientLightColor, glm::vec3(0.5, 0.5, 0.5));
	shader->setUniform(Shader::ENamedShaderUniform::SunDirection, glm::vec3(0, 1, 0));
	shader->setUniform(Shader::ENamedShaderUniform::SunColor, glm::vec3(0.5, 0.5, 0.5));

    // generate mesh for voxel(s)
    auto [vao, vertexCount] = makeVoxelVao(voxels);
    glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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