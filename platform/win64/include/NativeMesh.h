#pragma once
#include <glad/include/gl.h>
#include <vector>
#include <util/Constants.h>

namespace U {
	class NativeMesh {
	public:
		GLuint _vbo, _vao;

		const std::size_t _numVertices;

		NativeMesh(std::vector<vertex_t> vertices);
	};
}