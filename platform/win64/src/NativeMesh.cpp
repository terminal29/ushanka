#include <NativeMesh.h>
#include <platform/Mesh.h>
#include <glad/include/gl.h>
#include <platform/Shader.h>

U::NativeMesh::NativeMesh(std::vector<vertex_t> vertices) :_numVertices(vertices.size())
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// flatten vertices from whatever into a single vector
	std::vector<vertex_element_t> vertex_elements_flat;
	vertex_elements_flat.reserve(vertices.size() * elements_per_vertex);
	for (auto& vertex : vertices)
    {
        for (auto& element : vertex)
        {
			vertex_elements_flat.push_back(element);
		}
	}

    // Push to vram
    glBufferData(GL_ARRAY_BUFFER, vertex_elements_flat.size() * sizeof(decltype(vertex_elements_flat)::value_type), vertex_elements_flat.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(Shader::vertexPosAttribIndex);
    glVertexAttribPointer(Shader::vertexPosAttribIndex, elements_per_vertex_position, GL_FLOAT, GL_FALSE, vertex_stride, (void*)0);
            
    // vertex normals
    glEnableVertexAttribArray(Shader::vertexNormalAttribIndex);
    glVertexAttribPointer(Shader::vertexNormalAttribIndex, elements_per_vertex_normal, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(vertex_element_t) * elements_per_vertex_position));
            
    // vertex texture coords
    glEnableVertexAttribArray(Shader::vertexTextureCoordAttribIndex);
    glVertexAttribPointer(Shader::vertexTextureCoordAttribIndex, elements_per_texture_coodinate, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(vertex_element_t) * (elements_per_vertex_position + elements_per_vertex_normal)));

    glBindVertexArray(0);
}

