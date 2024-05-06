#include <platform/common/include/Shader.h>
#include "platform/win64/thirdparty/glad/include/gl.h"
#include <utility>
#include <iostream>


struct Shader::ShaderImpl {

    GLuint _shaderProgramObject = 0;

    GLuint _vao = 0;
    GLuint _vbo = 0;

	inline void makeVaoVbo(GLuint vertexPosAttribIndex, GLuint colorAttribIndex) {
        glGenBuffers(1, &_vbo);
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
		{
            {
                glBindBuffer(GL_ARRAY_BUFFER, _vbo);
                // 3 floats for position, 
                glEnableVertexAttribArray(vertexPosAttribIndex);
                glVertexAttribPointer(vertexPosAttribIndex, 3, GL_FLOAT, GL_FALSE, 2 * per_vertex * sizeof(value_t), (void*)0);
                // 3 floats for color
                glEnableVertexAttribArray(colorAttribIndex);
                glVertexAttribPointer(colorAttribIndex, 3, GL_FLOAT, GL_FALSE, 2 * per_vertex * sizeof(value_t), (void*)(per_vertex * sizeof(value_t)));
            }
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
        glBindVertexArray(0);
    }

    inline void makeShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {

        int success;
        char infoLog[512];
        // make vertex shader
		const char* vertexShaderCStr = vertexShader.c_str();
        GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderObject, 1, &vertexShaderCStr, NULL);
        glCompileShader(vertexShaderObject);
        glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderObject, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            std::terminate();
        }

        // make frament shader
        const char* fragmentShaderCStr = fragmentShader.c_str();
        GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderObject, 1, &fragmentShaderCStr, NULL);
        glCompileShader(fragmentShaderObject);
        glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderObject, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            std::terminate();
        }

        // make program
        _shaderProgramObject = glCreateProgram();
                
        glAttachShader(_shaderProgramObject, vertexShaderObject);
        glAttachShader(_shaderProgramObject, fragmentShaderObject);
        glLinkProgram(_shaderProgramObject);
        glGetProgramiv(_shaderProgramObject, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_shaderProgramObject, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            std::terminate();
        }

		// delete vertex & fragment shaders
        glDeleteShader(vertexShaderObject);
        glDeleteShader(fragmentShaderObject);
    }

	inline void bindVBO() {
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	}

    inline void bindVAO() {
        glBindVertexArray(_vao);
    }

	inline void pushBufferData(std::vector<value_t> bufferData) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(value_t) * bufferData.size(), bufferData.data(), GL_STATIC_DRAW);
	}

	inline void unbindVBO() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	inline void unbindVAO() {
		glBindVertexArray(0);
	}

    inline void bindShader() {
		glUseProgram(_shaderProgramObject);
    }

    inline void unbindShader() {
        glUseProgram(0);
    }


};

void Shader::ShaderImplDeleter::operator()(ShaderImpl* ptr)
{
    std::default_delete<ShaderImpl>()(ptr);
}

const std::string Shader::vertexPosAttribName{ "aPos" };
const std::string Shader::colorAttribName{ "aColor" };

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) :
    _pimpl(std::unique_ptr<ShaderImpl, ShaderImplDeleter>(new ShaderImpl()))
{
    _pimpl->makeVaoVbo(vertexPosAttribIndex, colorAttribIndex);
    _pimpl->makeShaderProgram(vertexShader, fragmentShader);
}


void Shader::drawVertices(const std::vector<vertex_t>& vertices, const RGBColor& color) const
{
    std::vector<value_t> vertexWithColor;
	for (const auto& vertex : vertices) {
        // x y z r g b
        vertexWithColor.insert(vertexWithColor.end(), { vertex.at(0), vertex.at(1), vertex.at(2), color.r / 255.f, color.g / 255.f, color.b / 255.f });
	}
    glEnable(GL_CULL_FACE);
	_pimpl->bindVBO();
	_pimpl->pushBufferData(vertexWithColor);
	_pimpl->unbindVBO();
    _pimpl->bindShader();
    _pimpl->bindVAO();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
	_pimpl->unbindVAO();
    _pimpl->unbindShader();

}

