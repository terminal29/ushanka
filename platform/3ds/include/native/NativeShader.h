#pragma once
#include <3ds.h>
#include <citro3d.h>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <optional>
#include <engine/render/Shader.h>
#include <engine/util/common/Constants.h>

namespace U::Engine::Render {
	class Renderer;
};


namespace U::Engine::Render::Native {
	class NativeRenderer;

	class NativeShader {
		friend class NativeRenderer;

		/*
			Vertex shader binary
		*/
		std::unique_ptr<DVLB_s> _vertexShaderDVLB;

		/*
			Shader program
		*/
		shaderProgram_s _shaderProgram;

		/*
			Fragment lighting structs
		*/
		C3D_LightEnv lightEnv;

		// Sun light
		C3D_Light light;

		C3D_LightLut lut_Spec;

		// Fragment shader "env" (?)
		C3D_TexEnv* texEnv;

		/*
			Default material
		*/
		C3D_Material _material;

		/*
			Uniforms to go to vertex shader
		*/
		std::unordered_map<Shader::ENamedShaderUniform, std::pair<std::string, int>> _vertexUniforms;

		/*
				mat3 uniforms to get converted to quat (vec4) to go to vertex shader
			*/
		std::unordered_map<Shader::ENamedShaderUniform, bool> _vec3ToQuatVertexUniforms;

		/*
				Callback overrides to go to fragment pipeline
			*/
		std::unordered_map<Shader::ENamedShaderUniform, std::function<void(const glm::fvec3&)>> _vec3FragmentUniforms;

		void setFragmentAmbientLightColor(const glm::fvec3& value) noexcept;
		void setFragmentSunDirection(const glm::fvec3& value) noexcept;
		void setFragmentSunColor(const glm::fvec3& value) noexcept;

	public:
		static C3D_Mtx glmFMatToC3DMtx(const glm::fmat4& value) noexcept
		{
			// column major -> c3d_mtx is row major
			const auto transposed_value = glm::transpose(value);
			C3D_Mtx result{};
			for (int i = 0; i < 4; i++) {
				result.r[i].x = transposed_value[i].x;
				result.r[i].y = transposed_value[i].y;
				result.r[i].z = transposed_value[i].z;
				result.r[i].w = transposed_value[i].w;
			}
			return result;
		}

		static glm::fmat4 c3DMtxToGlmFMat(const C3D_Mtx& value) noexcept
		{
			// column major -> c3d_mtx is row major
			glm::fmat4 result{};
			for (int i = 0; i < 4; i++) {
				result[i].x = value.r[i].x;
				result[i].y = value.r[i].y;
				result[i].z = value.r[i].z;
				result[i].w = value.r[i].w;
			}
			return glm::transpose(result);
		}

		static C3D_FVec glmFVecToC3DFVec(const glm::fvec4& value) noexcept
		{
			C3D_FVec vec;
			vec.x = value.x;
			vec.y = value.y;
			vec.z = value.z;
			vec.w = value.w;
			return vec;
		}

		void compile(
			const std::string& vertexShader /* empty */,
			const std::string& fragmentShader /* empty */,
			std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames);
		void bind() noexcept;
		void unbind() noexcept;

		void setUniformMat4(Shader::ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;
		void setUniformMat3(Shader::ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;
		void setUniformVec3(Shader::ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;
		void setUniformVec4(Shader::ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

		std::optional<int> getUniformLocation(Shader::ENamedShaderUniform uniform) noexcept;

		inline shaderProgram_s& getShaderProgram() noexcept { return _shaderProgram; }
	};
}