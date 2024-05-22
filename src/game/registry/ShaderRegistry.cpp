#include "game/registry/ShaderRegistry.h"
#include "platform/Shader.h"
#include <format>

namespace U {
	
	const std::unordered_map<ENamedShader, entt::hashed_string> NamedShaderIDs{
		{ENamedShader::Standard, "Standard"}
	};

	std::unordered_map<entt::hashed_string::hash_type, std::shared_ptr<Shader>> ShaderRegistry{};

	void initDefaultShaders() {
		std::unordered_map<Shader::ENamedShaderUniform, std::string> uniforms {
			{Shader::ENamedShaderUniform::ModelMatrix, "ModelMatrix"},
			{Shader::ENamedShaderUniform::ViewMatrix, "ViewMatrix"},
			{Shader::ENamedShaderUniform::ProjectionMatrix, "ProjectionMatrix"},
			{Shader::ENamedShaderUniform::NormalMatrix, "NormalMatrix"},
			{Shader::ENamedShaderUniform::AmbientLightColor, "AmbientLightColor"},
			{Shader::ENamedShaderUniform::SunDirection, "SunDirection"},
			{Shader::ENamedShaderUniform::SunColor, "SunColor"},
			{Shader::ENamedShaderUniform::Texture0, "Texture0"}
		};

		auto defaultshader = std::shared_ptr<Shader>(
			new Shader(R"(
					#version 330 core
					// position
					layout (location = 0) in vec3 VertexPosition;

					// normal	
					layout (location = 1) in vec3 VertexNormal;

					// texture coordinates
					layout (location = 2) in vec2 VertexTextureCoord;

					// color
					layout (location = 3) in vec4 VertexColor;

					// model
					uniform mat4 ModelMatrix;

					// view
					uniform mat4 ViewMatrix;

					// projection
					uniform mat4 ProjectionMatrix;

					// normal
					uniform mat3 NormalMatrix;

					out vec2 TexUV;
					out vec3 Normal;
					out vec4 Color;

					void main()
					{
						gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);
						TexUV = VertexTextureCoord;
						Normal = normalize(NormalMatrix * VertexNormal);
						Color = VertexColor;
					}
			)",
			R"(
				#version 330 core
					out vec4 FragColor;

					in vec2 TexUV;
					in vec3 Normal;
					in vec4 Color;

	                // ambient light color
					uniform float AmbientLightColor;

					// sun direction		
					uniform vec3 SunDirection;

					// sun color
					uniform vec4 SunColor;

					// texture 0
					uniform sampler2D Texture0;

					void main()
					{
						vec3 ambient = AmbientLightColor * SunColor.rgb;
						vec3 lightDir = normalize(-SunDirection);
						float diff = max(dot(Normal, lightDir), 0.0);
						vec3 diffuse = diff * SunColor.rgb;
						vec3 result = (ambient + diffuse) * Color.rgb;
						FragColor = vec4(result, 1.0) * texture(Texture0, TexUV);
					}
			)", uniforms));

		auto id = NamedShaderIDs.at(ENamedShader::Standard);
		ShaderRegistry.insert_or_assign(id.value(), defaultshader);
	}
}