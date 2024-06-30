#pragma once
#include "engine/util/common/Color.h"
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <engine/util/common/Constants.h>
#include <unordered_map>

namespace U::Engine::Render {

	namespace U::Native {
		class NativeShader;
	}

	class Shader {
		std::shared_ptr<U::Native::NativeShader> _impl;

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

		U::Native::NativeShader* getNativeShader();

		void bind() noexcept;

		void unbind() noexcept;

		void setUniformMat4(ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;

		void setUniformMat3(ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;

		void setUniformVec4(ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

		void setUniformVec3(ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;

	};
}