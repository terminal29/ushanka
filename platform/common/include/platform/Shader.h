#pragma once
#include "util/Color.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <util/Constants.h>
#include <map>

namespace U {

    class NativeShader;

    class Shader {
        std::shared_ptr<NativeShader> _impl;

    public:

        Shader(const std::string& vertexShader, const std::string& fragmentShader);
        ~Shader();

		std::shared_ptr<NativeShader> getNativeShader();

        void bind();
        void unbind();

		void setUniform(NamedShaderUniform uniform, const glm::mat4& value);
		void setUniform(NamedShaderUniform uniform, const glm::mat3& value);
		void setUniform(NamedShaderUniform uniform, const glm::vec3& value);
		void setUniform(NamedShaderUniform uniform, const glm::vec4& value);
		//void setUniform(const std::string& name, const std::shared_ptr<Texture>& texture);

		void drawArrays(const std::vector<vertex_t>& vertexData);




    };
}