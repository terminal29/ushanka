#pragma once
#include "util/Color.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <util/Constants.h>

namespace U {

    class NativeShader;

    class Shader {
        std::shared_ptr<NativeShader> _impl;

    public:

        static constexpr char vertexPosAttribName[] = "aPos";
        static constexpr uint8_t vertexPosAttribIndex{ 0 };

        static constexpr char vertexNormalAttribName[] = "aNorm" ;
        static constexpr uint8_t vertexNormalAttribIndex{ 1 };

        static constexpr char vertexTextureCoordAttribName[] = "aTexUV" ;
        static constexpr uint8_t vertexTextureCoordAttribIndex{ 2 };

        Shader(const std::string& vertexShader, const std::string& fragmentShader);
        ~Shader();

        void bind();
        void unbind();
		std::shared_ptr<NativeShader> getNativeShader();

    };
}