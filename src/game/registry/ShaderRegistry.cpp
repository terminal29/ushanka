#include "game/registry/ShaderRegistry.h"
#include <format>

namespace U {
	void initDefaultShaders() {

		auto defaultshader = std::shared_ptr<Shader>(
			new Shader(std::format(R"(
					#version 330 core
					// position
					layout (location = {0}) in vec3 {1};

					// normal	
					layout (location = {2}) in vec3 {3};

					// texture coordinates
					layout (location = {4}) in vec2 {5};

					// color
					layout (location = {6}) in vec4 {7};

					// model
					uniform mat4 {8};

					// view
					uniform mat4 {9};

					// projection
					uniform mat4 {10};

					// normal
					uniform mat3 {11};

					out vec2 TexUV;
					out vec3 Normal;
					out vec4 Color;

					void main()
					{
						gl_Position = {10} * {9} * {8} * vec4({1}, 1.0);
						TexUV = {5};
						Normal = normalize({11} * {3});
						Color = {7};
					}
			)", 
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexPosition).first,		// 0
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexPosition).second,		// 1
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexNormal).first,			// 2
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexNormal).second,		// 3
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexTextureCoord).first,	// 4
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexTextureCoord).second, // 5
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexColor).first,			// 6
				Shader::NamedShaderParams.at(Shader::NamedShaderParam::VertexColor).second,			// 7
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::ModelMatrix),			// 8
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::ViewMatrix),				// 9
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::ProjectionMatrix),		// 10
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::NormalMatrix)			// 11
				),
			std::format(R"(
				#version 330 core
					out vec4 FragColor;

					in vec2 TexUV;
					in vec3 Normal;
					in vec4 Color;

	                // ambient light color
					uniform float {0};

					// sun direction		
					uniform vec3 {1};

					// sun color
					uniform vec4 {2};

					// texture 0
					uniform sampler2D {3};

					void main()
					{
						vec3 ambient = {0} * {2}.rgb;
						vec3 lightDir = normalize(-{1});
						float diff = max(dot(Normal, lightDir), 0.0);
						vec3 diffuse = diff * {2}.rgb;
						vec3 result = (ambient + diffuse) * Color.rgb;
						FragColor = vec4(result, 1.0) * texture({3}, TexUV);
					}
			)",
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::AmbientLightColor), // 0
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::SunDirectionVector), // 1
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::SunColor), // 2
				Shader::NamedShaderUniforms.at(Shader::NamedShaderUniform::Texture0) // 3
				
				
				
				)));


		ShaderRegistry.load(NamedShader::Standard, defaultshader);
	}
}