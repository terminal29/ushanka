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

		void setUniformMat4(ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;

		void setUniformMat3(ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;

		void setUniformVec4(ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

		void setUniformVec3(ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;

	};
}