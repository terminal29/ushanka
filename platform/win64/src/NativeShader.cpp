#include "glad/include/gl.h"
#include <iostream>
#include <platform/Shader.h>
#include <utility>
#include <NativeShader.h>

void U::NativeShader::compile(const std::string& vertexShader, const std::string& fragmentShader) {
    int success;
    char infoLog[512];
    // make vertex shader
    const char* vertexShaderCStr = vertexShader.c_str();
    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderObject, 1, &vertexShaderCStr, NULL);
    glCompileShader(vertexShaderObject);
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderObject, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
        std::terminate();
    }

    // make frament shader
    const char* fragmentShaderCStr = fragmentShader.c_str();
    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObject, 1, &fragmentShaderCStr, NULL);
    glCompileShader(fragmentShaderObject);
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderObject, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
        std::terminate();
    }

    // delete vertex & fragment shaders
    glDeleteShader(vertexShaderObject);
    glDeleteShader(fragmentShaderObject);
}

void U::NativeShader::bind()
{
    glUseProgram(_shaderProgramObject);
}

    void U::NativeShader::unbind()
{
    glUseProgram(0);
}
