#pragma once
#include "platform/Renderer.h"

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


        std::shared_ptr<C3D_RenderTarget> _rtTopLeft{ nullptr };
        std::shared_ptr<Platform> _platform;


        std::unordered_map<Shader::ENamedVAOParameter, std::string> _vaoParamNames{
            {Shader::ENamedVAOParameter::VertexPosition, "VertexPosition"},
            {Shader::ENamedVAOParameter::VertexNormal, "VertexNormal"},
            {Shader::ENamedVAOParameter::VertexTextureCoord, "VertexTextureCoord"},
            {Shader::ENamedVAOParameter::VertexColor, "VertexColor"}
        };

        std::unordered_map<Shader::ENamedVAOParameter, int> _vaoParamPositions{
            {Shader::ENamedVAOParameter::VertexPosition, 0},
            {Shader::ENamedVAOParameter::VertexNormal, 1},
            {Shader::ENamedVAOParameter::VertexTextureCoord, 2},
            {Shader::ENamedVAOParameter::VertexColor, 3}
        };

    public:
        NativeRenderer(std::shared_ptr<Platform> platform);

        ~NativeRenderer();

        void frameBegin();

        void frameEnd();

        bool waitForVSync() const noexcept;

        void drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept;

        Size getWindowSize() const noexcept;

        // ~vao, ~vbo, vertexCount
        std::tuple<std::shared_ptr<C3D_AttrInfo>, std::shared_ptr<void>, std::size_t> makeVoxelVBOAttrs(const std::vector<U::Voxel>& voxels);
    };

}