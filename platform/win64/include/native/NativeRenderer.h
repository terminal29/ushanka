#pragma once
#include "engine/render/Renderer.h"

#define GLFW_INCLUDE_NONE
#include "glad/include/gl.h"
#include "glfw/include/glfw3.h"

#include <array>
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>


namespace U::Native {

    class NativeShader;

    class NativeRenderer {
        friend class NativeShader;

        std::shared_ptr<GLFWwindow> _window;
        std::shared_ptr<U::Engine::Platform::Platform> _platform;

        U::Engine::Util::Color _clearColor{ 3, 7, 7 };

        std::unordered_map<U::Engine::Render::Shader::ENamedVAOParameter, std::string> _vaoParamNames {
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexPosition, "VertexPosition"},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexNormal, "VertexNormal"},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexTextureCoord, "VertexTextureCoord"},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexColor, "VertexColor"}
        };

        std::unordered_map<U::Engine::Render::Shader::ENamedVAOParameter, GLuint> _vaoParamPositions{
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexPosition, 0},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexNormal, 1},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexTextureCoord, 2},
            {U::Engine::Render::Shader::ENamedVAOParameter::VertexColor, 3}
        };

    public:
        NativeRenderer(std::shared_ptr<U::Engine::Platform::Platform> platform);

        ~NativeRenderer();

        void onOpenGLError(int error, const char* description);

        void frameBegin();

        void frameEnd();

        bool waitForVSync() const noexcept;

		void drawVoxels(const U::Engine::Components::Camera& camera, const glm::ivec3& globalOffset, const std::vector<U::Engine::Render::Voxel::Voxel>& voxels) noexcept;

        U::Engine::Util::Size getWindowSize() const noexcept;

		void setClearColor(const U::Engine::Util::Color& color) noexcept;

        std::tuple<GLint, GLint, std::size_t> makeVoxelVaoVbo(const std::vector<U::Engine::Render::Voxel::Voxel>& voxels);
    };

}