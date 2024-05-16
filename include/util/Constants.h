#pragma once
#include <array>
#include <string_view>
#include <map>
#include <string>
#include <cinttypes>

namespace U {

    static constexpr char app_title[] = "Ushanka";

    using vertex_element_t = float;
    
    static constexpr int elements_per_vertex_position = 3;

    static constexpr int elements_per_vertex_normal = 3;

    static constexpr int elements_per_texture_coodinate = 2;

	static constexpr int elements_per_vertex_color = 3;

    static constexpr int elements_per_vertex = elements_per_vertex_position + elements_per_vertex_normal + elements_per_texture_coodinate + elements_per_vertex_color;

	static constexpr int vertex_stride = elements_per_vertex * sizeof(vertex_element_t);

    using vertex_t = std::array<vertex_element_t, elements_per_vertex>;

    // just some arbitrary number until more testing done on 3ds
    static constexpr std::size_t maxVertices = 200;

	enum class NamedShaderParam {
		VertexPosition,
		VertexNormal,
		VertexTextureCoord,
		VertexColor,
	};

	enum class NamedShaderUniform {
		ModelMatrix,
		ViewMatrix,
		ProjectionMatrix,
		NormalMatrix,
		AmbientLightColor,
		SunDirectionVector,
		SunColor,
		Texture0
	};

	extern const std::map<NamedShaderParam, std::pair<std::string, uint8_t>> NamedShaderParams;

	extern const std::map<NamedShaderUniform, std::string> NamedShaderUniforms;
}