#include <platform/Shader.h>
#include <NativeShader.h>

U::Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    : _impl(std::shared_ptr<U::NativeShader>(new U::NativeShader()))
{
    _impl->compile(vertexShader, fragmentShader);
}

void U::Shader::bind() {
	_impl->bind();
}

void U::Shader::unbind() {
	_impl->unbind();
}

std::shared_ptr<U::NativeShader> U::Shader::getNativeShader() {
	return _impl;
}

U::Shader::~Shader() = default;