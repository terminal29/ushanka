#pragma once
#include "include/util/Color.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class Shader {
    struct ShaderImpl;
    struct ShaderImplDeleter {
        void operator()(ShaderImpl* p);
    };
    std::unique_ptr<ShaderImpl, ShaderImplDeleter> _pimpl { nullptr };

public:
    using vertex_element_t = float;
    static const int per_vertex = 3;
    using vertex_t = std::array<vertex_element_t, per_vertex>;
    // just some arbitrary number until more testing done on 3ds
    static const unsigned int maxVertices { 200 };

    static const std::string vertexPosAttribName;
    static const uint8_t vertexPosAttribIndex { 0 };

    static const std::string colorAttribName;
    static const uint8_t colorAttribIndex { 1 };

    // constructor builds the shader
    Shader(const std::string& vertexShader, const std::string& fragmentShader);

    // quads (triangle_strip)
    void drawVertices(const std::vector<vertex_t>& vertices, const RGBColor& color) const;
};