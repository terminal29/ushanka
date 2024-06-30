#include <3ds.h>
#include "shader_shbin.h"
#include <NativeShader.h>
#include <iostream>
#include <platform/Shader.h>
#include <utility>
#include <limits>
#include <iomanip>

void U::NativeShader::compile(
    const std::string& vertexShader,
    const std::string& fragmentShader,
    std::unordered_map<Shader::ENamedShaderUniform, std::string> uniformNames)
{
    // todo: support runtime compilation of shaders
    _vertexShaderDVLB = std::unique_ptr<DVLB_s>(DVLB_ParseFile((u32*)shader_shbin, shader_shbin_size));
    shaderProgramInit(&_shaderProgram);
    shaderProgramSetVsh(&_shaderProgram, &_vertexShaderDVLB->DVLE[0]);

    for (auto& [name, uniform] : uniformNames) {
        switch (name) {
        case Shader::ENamedShaderUniform::NormalMatrix: {
            int loc = shaderInstanceGetUniformLocation(_shaderProgram.vertexShader, "NormalQuat");
            if (loc == -1) {
                std::cout << "NormalQuat not found in shader\n";
                while (true) {
                    std::cout << "";
                };
            }
            // Override with NormalQuat
            _vertexUniforms.insert_or_assign(name, std::pair<std::string, int>{ uniform, loc });
            // add to list so we know to convert to quat before sending
            _vec3ToQuatVertexUniforms.insert_or_assign(name, true);

        } break;
        case Shader::ENamedShaderUniform::AmbientLightColor: {
            _vec3FragmentUniforms.insert_or_assign(name, [this](const glm::fvec3& value) {
                setFragmentAmbientLightColor(value);
            });
        } break;
        case Shader::ENamedShaderUniform::SunDirection: {
            _vec3FragmentUniforms.insert_or_assign(name, [this](const glm::fvec3& value) {
                setFragmentSunDirection(value);
            });
        } break;
        case Shader::ENamedShaderUniform::SunColor: {
            _vec3FragmentUniforms.insert_or_assign(name, [this](const glm::fvec3& value) {
                setFragmentSunColor(value);
            });
        } break;
        case Shader::ENamedShaderUniform::Texture0: {
            // todo: implement
        } break;
        default: {
            int loc = shaderInstanceGetUniformLocation(_shaderProgram.vertexShader, uniform.c_str());
            if (loc == -1) {
                std::cout << uniform.c_str() << " not found in shader\n";
                while (true) {
                    std::cout << "";
                };
            }
            _vertexUniforms.insert_or_assign(name, std::pair<std::string, int>{ uniform, loc });
        } break;
        }
    }

    // Setup lighting
    _material = {
        { 0.1f, 0.1f, 0.1f }, //ambient
        { 0.4f, 0.4f, 0.4f }, //diffuse
        { 0.5f, 0.5f, 0.5f }, //specular0
        { 0.0f, 0.0f, 0.0f }, //specular1
        { 1.0f, 1.0f, 1.0f }, //emission
    };

    C3D_LightEnvInit(&lightEnv);
    C3D_LightEnvBind(&lightEnv);
    C3D_LightEnvMaterial(&lightEnv, &_material);

    LightLut_Phong(&lut_Spec, 30);
    C3D_LightEnvLut(&lightEnv, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &lut_Spec);

    C3D_FVec lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);

    C3D_LightInit(&light, &lightEnv);
    C3D_LightColor(&light, 1.0, 1.0, 1.0);
    C3D_LightPosition(&light, &lightVec);

    texEnv = C3D_GetTexEnv(0);
    C3D_TexEnvInit(texEnv);
    C3D_TexEnvSrc(texEnv, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR);
    C3D_TexEnvFunc(texEnv, C3D_Both, GPU_ADD);
}

void U::NativeShader::bind() noexcept
{
    C3D_BindProgram(&_shaderProgram);
    C3D_LightEnvBind(&lightEnv);
    C3D_SetTexEnv(0, texEnv);
}

void U::NativeShader::unbind() noexcept
{
    // unbinding not implemented in citro3d
}

void U::NativeShader::setUniformMat4(Shader::ENamedShaderUniform uniform, const glm::fmat4& value) noexcept
{
    C3D_Mtx c3dMat = glmFMatToC3DMtx(value);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, _vertexUniforms.at(uniform).second, &c3dMat);
}

void U::NativeShader::setUniformMat3(Shader::ENamedShaderUniform uniform, const glm::fmat3& value) noexcept
{
    glm::fmat4 mat4 = glm::fmat4(1);
    C3D_Mtx c3dMat = glmFMatToC3DMtx(mat4);
    if (uniform == Shader::ENamedShaderUniform::NormalMatrix) {
        // convert to quaternion
        C3D_FQuat c3dQuat = Quat_FromMtx(&c3dMat);
        // Set as vec4
        C3D_FVUnifSet(GPU_VERTEX_SHADER, _vertexUniforms.at(uniform).second, c3dQuat.x, c3dQuat.y, c3dQuat.z, c3dQuat.w);
    } else {
        // Set as 3x4 (w will be 0);
        C3D_FVUnifMtx3x4(GPU_VERTEX_SHADER, _vertexUniforms.at(uniform).second, &c3dMat);
    }
}

void U::NativeShader::setUniformVec3(Shader::ENamedShaderUniform uniform, const glm::fvec3& value) noexcept
{
    // Special handlers for fragment uniforms
    if (_vec3FragmentUniforms.contains(uniform)) {
        _vec3FragmentUniforms.at(uniform)(value);
    } else {
        C3D_FVUnifSet(GPU_VERTEX_SHADER, _vertexUniforms.at(uniform).second, value.x, value.y, value.z, 1.0f);
    }
}

void U::NativeShader::setUniformVec4(Shader::ENamedShaderUniform uniform, const glm::fvec4& value) noexcept
{
    C3D_FVUnifSet(GPU_VERTEX_SHADER, _vertexUniforms.at(uniform).second, value.x, value.y, value.z, value.w);
}

void U::NativeShader::setFragmentAmbientLightColor(const glm::fvec3& value) noexcept
{
    C3D_LightEnvAmbient(&lightEnv, value.x, value.y, value.z);
}

void U::NativeShader::setFragmentSunDirection(const glm::fvec3& value) noexcept
{
    C3D_FVec lightVec = FVec4_New(value.x, value.y, value.z, 1.0f);
    C3D_LightPosition(&light, &lightVec);
}

void U::NativeShader::setFragmentSunColor(const glm::fvec3& value) noexcept
{
    C3D_LightColor(&light, value.x, value.y, value.z);
}

std::optional<int> U::NativeShader::getUniformLocation(Shader::ENamedShaderUniform uniform) noexcept
{
	return _vertexUniforms.contains(uniform) ? std::optional<int>(_vertexUniforms.at(uniform).second) : std::nullopt;
}