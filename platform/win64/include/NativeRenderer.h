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
    class NativeRenderer {

        std::shared_ptr<GLFWwindow> _window;
        std::shared_ptr<Platform> _platform;

    public:
        NativeRenderer(std::shared_ptr<Platform> platform);

        ~NativeRenderer();

        void onOpenGLError(int error, const char* description);

        void frameBegin();

        void drawQuad(const Point& topLeft, const Size& size, const RGBColor& color) noexcept;

        void drawVertices(const Camera& camera, const std::vector<glm::vec3>& vertices, const RGBColor& color) noexcept;

        void frameEnd();

        bool waitForVSync() const noexcept;

        void draw(const Camera& camera, const Transform& objectTransform, const std::shared_ptr<U::Mesh> mesh, const std::shared_ptr<U::Shader> shader) noexcept;

        Size getWindowSize() const noexcept;
    };

}