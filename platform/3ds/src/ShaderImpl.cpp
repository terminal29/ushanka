#include "shader_shbin.h"
#include <3ds.h>
#include <citro3d.h>
#include <functional>
#include <iostream>
#include <memory>
#include <platform/common/include/Shader.h>
#include <utility>

//static const u32 DISPLAY_TRANSFER_FLAGS = (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

struct Shader::ShaderImpl {

    std::function<void(DVLB_s*)> DVLBDeleter = [](DVLB_s* ptr) {
        DVLB_Free(ptr);
    };

    shaderProgram_s _shaderProgram;
    C3D_BufInfo _vboBufInfo;
    C3D_AttrInfo _vboAttrInfo;
    C3D_TexEnv* _shaderTexEnv; // (fragment shader)

    // todo unique_ptr this
    /*value_t*/ void* _vboDataPos { nullptr };
    /*value_t*/ void* _vboDataColor { nullptr };

    std::unique_ptr<DVLB_s, decltype(DVLBDeleter)> vshader_dvlb { nullptr };
    inline void makeShaderProgram()
    {
        // make shader program
        vshader_dvlb = std::unique_ptr<DVLB_s, decltype(DVLBDeleter)>(DVLB_ParseFile((u32*)shader_shbin, shader_shbin_size), DVLBDeleter);
        shaderProgramInit(&_shaderProgram);
        shaderProgramSetVsh(&_shaderProgram, &vshader_dvlb->DVLE[0]);
        C3D_BindProgram(&_shaderProgram);
    }

    inline void makeAttrVbo(uint8_t vertexPosAttribIndex, uint8_t colorAttribIndex)
    {
        // Add loaders for uniforms
        AttrInfo_Init(&_vboAttrInfo);
        AttrInfo_AddLoader(&_vboAttrInfo, vertexPosAttribIndex, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(&_vboAttrInfo, colorAttribIndex, GPU_FLOAT, 4); // v1=color

        // Configure buffers
        BufInfo_Init(&_vboBufInfo);

        // Configure the first fragment shading substage to just pass through the vertex color
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        _shaderTexEnv = C3D_GetTexEnv(0);
        C3D_TexEnvInit(_shaderTexEnv);
        C3D_TexEnvSrc(_shaderTexEnv, C3D_Both, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(_shaderTexEnv, C3D_Both, GPU_REPLACE);
    }

    inline void bindVBO()
    {
        C3D_SetAttrInfo(&_vboAttrInfo);
        C3D_SetBufInfo(&_vboBufInfo);
        C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
        // check correct order
        C3D_CullFace(GPU_CULL_BACK_CCW);
    }

    // Todo: optimise this shit
    inline void pushBufferData(std::vector<value_t> vertices, std::vector<value_t> colors)
    {
        // free old data
        if (_vboDataPos)
            linearFree(_vboDataPos);
        // push new data
        _vboDataPos = linearAlloc(vertices.size() * sizeof(value_t));
        memcpy(_vboDataPos, vertices.data(), vertices.size() * sizeof(value_t));
        BufInfo_Add(&_vboBufInfo, _vboDataPos, sizeof(value_t), 1, 0x0);

        // free old data
        if (_vboDataColor)
            linearFree(_vboDataColor);
        // push new data
        _vboDataColor = linearAlloc(colors.size() * sizeof(value_t));
        memcpy(_vboDataColor, colors.data(), colors.size() * sizeof(value_t));
        BufInfo_Add(&_vboBufInfo, _vboDataColor, sizeof(value_t), 1, 0x1);
    }

    inline void draw(std::size_t count)
    {
        C3D_DrawArrays(GPU_TRIANGLE_STRIP, 0, count);
    }

    inline void unbindVBO()
    {
        // Cant actually unbind
        // C3D_SetAttrInfo(nullptr);
        // C3D_SetBufInfo(nullptr);
    }

    inline void bindShader()
    {
        C3D_BindProgram(&_shaderProgram);
    }

    inline void unbindShader()
    {
        // cant actually unbind
        // C3D_BindProgram(&_shaderProgram);
    }
};

void Shader::ShaderImplDeleter::operator()(ShaderImpl* ptr)
{
    std::default_delete<ShaderImpl>()(ptr);
}

const std::string Shader::vertexPosAttribName { "inpos" };
const std::string Shader::colorAttribName { "inclr" };

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    : _pimpl(std::unique_ptr<ShaderImpl, ShaderImplDeleter>(new ShaderImpl()))
{
    _pimpl->makeShaderProgram();
    _pimpl->makeAttrVbo(vertexPosAttribIndex, colorAttribIndex);
}

void Shader::drawVertices(const std::vector<vertex_t>& vertices, const RGBColor& color) const
{
    std::vector<value_t> vertexFlat;
    std::vector<value_t> colorFlat;
    for (const auto& vertex : vertices) {
        // x y z
        vertexFlat.insert(vertexFlat.end(), { vertex.at(0), vertex.at(1), vertex.at(2) });
        // r g b
        colorFlat.insert(colorFlat.end(), { color.r / 255.f, color.g / 255.f, color.b / 255.f });
    }
    _pimpl->bindShader();
    _pimpl->bindVBO();
    _pimpl->pushBufferData(vertexFlat, colorFlat);
    _pimpl->draw(vertices.size());
    _pimpl->unbindVBO();
    _pimpl->unbindShader();
}
