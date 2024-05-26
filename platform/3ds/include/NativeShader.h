#pragma once 
#include <util/Constants.h>
#include <platform/Shader.h>
#include <3ds.h>
#include <citro3d.h>
#include <memory>
#include <functional>

namespace U {
	class Renderer;
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

		/*
			Default material
		*/
		C3D_Material _material;

		/*
			Uniforms to go to vertex shader
		*/
		std::unordered_map<Shader::ENamedShaderUniform, int> _vertexUniforms;

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

		C3D_Mtx glmFMatToC3DMtx(const glm::fmat4& value) noexcept {
			C3D_Mtx result{};
			for (int i = 0; i < T::length(); i++) {
				for (int j = 0; j < T::length(); j++) {
					result.r[i][j] = value[j][i];
				}
			}
			return result;
		}

		C3D_FVec glmFVecToC3DFVec(const glm::fvec4& value) noexcept {
			return { value.w, value.x, value.y, value.z };
		}

	public:
		void compile(
			const std::string& vertexShader /* points to vshader_shbin */,
			const std::string& fragmentShader /* empty */,
			std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames
		);
		void bind() noexcept;
		void unbind() noexcept;

		void setUniformMat4(Shader::ENamedShaderUniform uniform, const glm::fmat4& value) noexcept;
		void setUniformMat3(Shader::ENamedShaderUniform uniform, const glm::fmat3& value) noexcept;
		void setUniformVec3(Shader::ENamedShaderUniform uniform, const glm::fvec3& value) noexcept;
		void setUniformVec4(Shader::ENamedShaderUniform uniform, const glm::fvec4& value) noexcept;

	};
}