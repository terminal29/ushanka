#include "util/Constants.h"
namespace U {

	const std::map<NamedShaderParam, std::pair<std::string, uint8_t>> NamedShaderParams = {
			{NamedShaderParam::VertexPosition, { "aPos", 0 }},
			{NamedShaderParam::VertexNormal, { "aNorm", 1 }},
			{NamedShaderParam::VertexTextureCoord, { "aTexUV", 2 }},
			{NamedShaderParam::VertexColor, { "aColor", 3 }},
	};

	const std::map<NamedShaderUniform, std::string> NamedShaderUniforms = {
		{NamedShaderUniform::Texture0, "texture0"}, // sampler2D
		{NamedShaderUniform::ModelMatrix, "model"}, // mat4
		{NamedShaderUniform::ViewMatrix, "view"}, // mat4
		{NamedShaderUniform::ProjectionMatrix, "projection"}, // mat4
		{NamedShaderUniform::NormalMatrix, "normalMatrix"}, // mat3
		{NamedShaderUniform::AmbientLightColor, "ambientLightColor"}, // vec3
		{NamedShaderUniform::SunDirectionVector, "sunDirection"}, //vec3
		{NamedShaderUniform::SunColor, "sunColor"} //vec3
	};
}