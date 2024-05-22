#pragma once
#include "util/Color.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <util/Constants.h>
#include <unordered_map>

namespace U {

	class NativeShader;

	class Shader {
		friend class NativeShader;
		std::shared_ptr<NativeShader> _impl;


	public:

		enum class ENamedShaderUniform {
			ModelMatrix = 0,
			ViewMatrix,
			ProjectionMatrix,
			NormalMatrix,
			AmbientLightColor,
			SunDirection,
			SunColor,
			Texture0
		};

		enum class ENamedVAOParameter {
			VertexPosition = 0,
			VertexNormal,
			VertexTextureCoord,
			VertexColor
		};

		Shader(
			const std::string& vertexShader,
			const std::string& fragmentShader,
			std::unordered_map<ENamedShaderUniform, std::string> uniformNames
		);

		~Shader();

		std::shared_ptr<NativeShader> getNativeShader();

		void bind() noexcept;

		void unbind() noexcept;

		void setUniform(ENamedShaderUniform uniform, const glm::mat4& value) noexcept;

		void setUniform(ENamedShaderUniform uniform, const glm::mat3& value) noexcept;

		void setUniform(ENamedShaderUniform uniform, const glm::vec4& value) noexcept;

		void setUniform(ENamedShaderUniform uniform, const glm::vec3& value) noexcept;

	};
}