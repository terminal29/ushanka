#pragma once
#include "platform/Renderer.h"

#define GLFW_INCLUDE_NONE
#include "glad/include/gl.h"
#include "glfw/include/glfw3.h"

#include <array>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>

#include "platform/Shader.h"
#include "components/Transform.h"

namespace U {
    class NativeShader;

    class NativeRenderer {
        friend class NativeShader;

        std::shared_ptr<GLFWwindow> _window;
        std::shared_ptr<Platform> _platform;


        std::unordered_map<Shader::ENamedVAOParameter, std::string> _vaoParamNames {
            {Shader::ENamedVAOParameter::VertexPosition, "VertexPosition"},
            {Shader::ENamedVAOParameter::VertexNormal, "VertexNormal"},
            {Shader::ENamedVAOParameter::VertexTextureCoord, "VertexTextureCoord"},
            {Shader::ENamedVAOParameter::VertexColor, "VertexColor"}
        };

        std::unordered_map<Shader::ENamedVAOParameter, GLuint> _vaoParamPositions{
            {Shader::ENamedVAOParameter::VertexPosition, 0},
            {Shader::ENamedVAOParameter::VertexNormal, 1},
            {Shader::ENamedVAOParameter::VertexTextureCoord, 2},
            {Shader::ENamedVAOParameter::VertexColor, 3}
        };

    public:
        NativeRenderer(std::shared_ptr<Platform> platform);

        ~NativeRenderer();

        void onOpenGLError(int error, const char* description);

        void frameBegin();

        void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept;

        void drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept;

        void frameEnd();

        bool waitForVSync() const noexcept;

        //void draw(const Camera& camera, const Transform& objectTransform, const std::shared_ptr<U::Mesh> mesh, const std::shared_ptr<U::Shader> shader) noexcept;

		void drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept;

        Size getWindowSize() const noexcept;

        std::tuple<GLint, GLint, std::size_t> makeVoxelVaoVbo(const std::vector<U::Voxel>& voxels);
    };

}