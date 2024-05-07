#include "shader_shbin.h"
#include <3ds.h>
#include <citro3d.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <platform/common/include/Shader.h>
#include <source_location>
#include <utility>

struct vertex_pack_t {
    float pos[3];
    float color[4];
};
static_assert(std::is_pod<vertex_pack_t>::value);

u32 createRGBA(int r, int g, int b, int a)
{
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        + (a & 0xff);
}

struct Shader::ShaderImpl {

    // DVLB
    // (D.M.P. VectorGraphics Language Bytecode)
    DVLB_s* _vertexShaderDVLB = nullptr;
    shaderProgram_s _shaderProgram;
    C3D_BufInfo* _vboBufInfo;
    C3D_AttrInfo* _attributeInfo = nullptr;
    C3D_TexEnv* _shaderTexEnv = nullptr;
    C3D_LightEnv lightEnv;
    C3D_Light light;
    C3D_LightLut lut_Phong;

    const C3D_Material material = {
        { 0.0f, 0.0f, 0.0f }, //ambient
        { 1.0f, 1.0f, 1.0f }, //diffuse
        { 0.4f, 0.4f, 0.4f }, //specular0
        { 0.0f, 0.0f, 0.0f }, //specular1
        { 0.0f, 0.0f, 0.0f }, //emission
    };

    /* vertex_pack_t*/
    void* _vboDataPosColor { nullptr };

    inline void makeShaderProgram()
    {
        // make shader program
        _vertexShaderDVLB = DVLB_ParseFile((u32*)shader_shbin, shader_shbin_size);
        shaderProgramInit(&_shaderProgram);
        shaderProgramSetVsh(&_shaderProgram, &_vertexShaderDVLB->DVLE[0]);
    }

    inline void makeAttrVbo(uint8_t vertexPosAttribIndex, uint8_t colorAttribIndex)
    {
        // bind before we can get any attrs
        C3D_BindProgram(&_shaderProgram);

        // Add loaders for uniforms
        _attributeInfo = C3D_GetAttrInfo();
        AttrInfo_Init(_attributeInfo);
        AttrInfo_AddLoader(_attributeInfo, 0, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(_attributeInfo, 1, GPU_FLOAT, 4); // v1=color

        // Configure buffers
        _vboBufInfo = C3D_GetBufInfo();
        BufInfo_Init(_vboBufInfo);
        // allocate space for max vertices
        // Todo make this reallocation dynamic
        _vboDataPosColor = linearAlloc(maxVertices * sizeof(vertex_pack_t));
        BufInfo_Add(_vboBufInfo, _vboDataPosColor, sizeof(vertex_pack_t), 2, 0x10);

        // Configure the first fragment shading substage to just pass through the vertex color
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        _shaderTexEnv = C3D_GetTexEnv(0);
        C3D_TexEnvInit(_shaderTexEnv);
        C3D_TexEnvSrc(_shaderTexEnv, C3D_Both, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(_shaderTexEnv, C3D_Both, GPU_REPLACE);

        //https://github.com/devkitPro/3ds-examples/blob/979ab794de89f1ce30a2e307a8dc17fc4fe1d684/graphics/gpu/normal_mapping/source/main.c

        C3D_LightEnvInit(&lightEnv);
        C3D_LightEnvBind(&lightEnv);
        C3D_LightEnvMaterial(&lightEnv, &material);
        C3D_LightEnvBumpMode(&lightEnv, GPU_BUMP_AS_BUMP);
        C3D_LightEnvBumpSel(&lightEnv, 1);

        LightLut_Phong(&lut_Phong, 30);
        C3D_LightEnvLut(&lightEnv, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &lut_Phong);

        C3D_FVec lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);

        C3D_LightInit(&light, &lightEnv);
        C3D_LightColor(&light, 1.0, 1.0, 1.0);
        C3D_LightPosition(&light, &lightVec);
    }

    inline void bindVBO()
    {
        C3D_SetAttrInfo(_attributeInfo);
        C3D_SetBufInfo(_vboBufInfo);
        C3D_LightEnvBind(&lightEnv);
        //C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
        // check correct order
        //C3D_CullFace(GPU_CULL_BACK_CCW);
    }

    // Todo: optimise this shit
    inline void pushBufferData(const std::vector<vertex_t>& vertices, const RGBColor& color)
    {
        if (vertices.size() > maxVertices) {
            std::cout << "too many vertices (" << vertices.size() << " > " << maxVertices << ")" << std::endl;
            std::terminate();
        }

        // copy data into buffer
        std::vector<vertex_pack_t> flat;
        flat.reserve(maxVertices);
        for (const auto vertex : vertices) {
            flat.push_back(vertex_pack_t { { vertex[0], vertex[1], vertex[2] }, { color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f } });
        }
        // copy from c++ vec into vbo
        std::memcpy(_vboDataPosColor, flat.data(), flat.size() * sizeof(vertex_pack_t));
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
    _pimpl->bindShader();
    _pimpl->bindVBO();
    _pimpl->pushBufferData(vertices, color);
    _pimpl->draw(vertices.size());
}
