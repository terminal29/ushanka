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

    std::size_t offset = 0;
	constexpr std::size_t vertex_stride = 3;
	constexpr std::size_t normal_stride = 3;
	constexpr std::size_t texture_coord_stride = 2;
	constexpr std::size_t color_stride = 3;
	constexpr std::size_t stride = (vertex_stride + normal_stride + texture_coord_stride + color_stride) * sizeof(vertex_element_t);

    // vertex positions
    glEnableVertexAttribArray(NamedShaderParams.at(NamedShaderParam::VertexPosition).second);
    glVertexAttribPointer(NamedShaderParams.at(NamedShaderParam::VertexPosition).second, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	offset += (vertex_stride * sizeof(vertex_element_t));

	// vertex normals
    glEnableVertexAttribArray(NamedShaderParams.at(NamedShaderParam::VertexNormal).second);
    glVertexAttribPointer(NamedShaderParams.at(NamedShaderParam::VertexNormal).second, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (normal_stride * sizeof(vertex_element_t));
            
    // tex coord
    glEnableVertexAttribArray(NamedShaderParams.at(NamedShaderParam::VertexTextureCoord).second);
    glVertexAttribPointer(NamedShaderParams.at(NamedShaderParam::VertexTextureCoord).second, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (texture_coord_stride * sizeof(vertex_element_t));

    // vertex color
    glEnableVertexAttribArray(NamedShaderParams.at(NamedShaderParam::VertexColor).second);
    glVertexAttribPointer(NamedShaderParams.at(NamedShaderParam::VertexColor).second, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += (color_stride * sizeof(vertex_element_t));

    glBindVertexArray(0);
}

