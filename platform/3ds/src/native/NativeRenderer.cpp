#include "game/registry/ShaderRegistry.h"
#include "platform/Platform.h"
#include "platform/Renderer.h"
#include "platform/Shader.h"
#include <3ds.h>
#include <NativeRenderer.h>
#include <NativeShader.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <platform/Timer.h>
#include <util/Constants.h>
#include <util/Size.h>


inline constexpr U::Size ThreeDSWindowSize { 400, 240 };
inline constexpr float ThreeDSAspectRatio = static_cast<float>(ThreeDSWindowSize.width) / static_cast<float>(ThreeDSWindowSize.height);

inline constexpr u32 COMMON_DISPLAY_TRANSFER_FLAGS = (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

u32 _createRGBA(int r, int g, int b, int a)
{
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        + (a & 0xff);
}

U::NativeRenderer::NativeRenderer(std::shared_ptr<Platform> platform)
    : _platform(platform)
{
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    _rtTopLeft = std::shared_ptr<C3D_RenderTarget>(C3D_RenderTargetCreate(ThreeDSWindowSize.height, ThreeDSWindowSize.width, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8), [](C3D_RenderTarget* target) {
        if (target != nullptr)
            C3D_RenderTargetDelete(target);
    });
    // cornflower blue
    C3D_RenderTargetClear(_rtTopLeft.get(), C3D_CLEAR_ALL, 0x68B0D8FF, 0);
    C3D_RenderTargetSetOutput(_rtTopLeft.get(), GFX_TOP, GFX_LEFT, COMMON_DISPLAY_TRANSFER_FLAGS);
}

U::NativeRenderer::~NativeRenderer()
{
    C3D_Fini();
    gfxExit();
}

void U::NativeRenderer::frameBegin()
{
    auto ms = GlobalTimer.msSinceStart;
    u32 colorAsInt = _createRGBA(_clearColor.r, _clearColor.g, _clearColor.b, 255);
    C3D_RenderTargetClear(_rtTopLeft.get(), C3D_CLEAR_ALL, colorAsInt, 0);
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_FrameDrawOn(_rtTopLeft.get());
}

void U::NativeRenderer::frameEnd()
{
    //printf("Ushanka game by jaocb1\n");
    printf("CPU:     %6.2f%%\n", C3D_GetProcessingTime() * 6.0f);
    printf("GPU:     %6.2f%%\n", C3D_GetDrawingTime() * 6.0f);
    printf("CmdBuf:  %6.2f%%\n", C3D_GetCmdBufUsage() * 100.0f);
    printf("Free:    %zu%\n", osGetMemRegionFree(MEMREGION_ALL));
    //std::cout.flush();
    std::cout << "\x1B[H";
    C3D_FrameEnd(0);
}

bool U::NativeRenderer::waitForVSync() const noexcept
{
    gspWaitForVBlank();
    return aptMainLoop();
}

U::Size U::NativeRenderer::getWindowSize() const noexcept
{
    return ThreeDSWindowSize;
}

void U::NativeRenderer::drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<Voxel>& voxels) noexcept
{
    if (voxels.empty()) {
        std::cout << "voxels is empty"
                  << "\n";
        return;
    }
    auto shader = ShaderRegistry::Registry.at(voxels[0].shaderID);
    if (!shader) {
        std::cout << "shader not found"
                  << "\n";
        return;
    }
    shader->bind();

    //todo check these
    const glm::fmat4 model = glm::translate(glm::fmat4(1), glm::fvec3(globalOffset.x, globalOffset.y, globalOffset.z));

    const glm::fmat4 view = camera.getViewMatrix();

    C3D_Mtx projectionC3D;
    Mtx_PerspTilt(&projectionC3D, camera.fov * M_PI / 180.0f, ThreeDSAspectRatio, camera.nearPlane, camera.farPlane, false);
	auto projection = NativeShader::c3DMtxToGlmFMat(projectionC3D);


    /*auto perspectiveUniform = shader->getNativeShader()->getUniformLocation(Shader::ENamedShaderUniform::ProjectionMatrix);
    if (perspectiveUniform) {
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, *perspectiveUniform, &projection);
    }*/

    //auto viewUniform = shader->getNativeShader()->getUniformLocation(Shader::ENamedShaderUniform::ViewMatrix);
    //if (viewUniform) {
    //    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, *viewUniform, &view);
    //}

	//glm::fmat4 leftEyeProjection = NativeShader::c3DMtxToGlmFMat(projection);


    auto [vao, vbo, vertexCount] = makeVoxelVBOAttrs(voxels);
    // bind ~vao
    C3D_SetAttrInfo(&vao);
    // bind ~vbo
    C3D_SetBufInfo(&vbo.bufferInfo);

    shader->setUniformMat4(Shader::ENamedShaderUniform::ModelMatrix, model);
    shader->setUniformMat4(Shader::ENamedShaderUniform::ViewMatrix, view);
    shader->setUniformMat4(Shader::ENamedShaderUniform::ProjectionMatrix, projection);
    shader->setUniformMat3(Shader::ENamedShaderUniform::NormalMatrix, glm::fmat3(glm::transpose(glm::inverse(view * model))));
    shader->setUniformVec3(Shader::ENamedShaderUniform::AmbientLightColor, glm::fvec3(0.5, 0.5, 0.5));
    shader->setUniformVec3(Shader::ENamedShaderUniform::SunDirection, glm::fvec3(0, 1, 0));
    shader->setUniformVec3(Shader::ENamedShaderUniform::SunColor, glm::fvec3(0.5, 0.5, 0.5));

    C3D_DrawArrays(GPU_TRIANGLES, 0, 36);
    shader->unbind();
}

std::tuple<C3D_AttrInfo, U::NativeBufferWrapper, std::size_t> U::NativeRenderer::makeVoxelVBOAttrs(const std::vector<U::Voxel>& voxels) noexcept
{
    // Make attribute info (but dont bind it yet)
    C3D_AttrInfo attrInfo;
    AttrInfo_Init(&attrInfo);
    AttrInfo_AddLoader(&attrInfo, 0, GPU_FLOAT, 3); // v0=position
    AttrInfo_AddLoader(&attrInfo, 1, GPU_FLOAT, 3); // v1=normal
    AttrInfo_AddLoader(&attrInfo, 2, GPU_FLOAT, 2); // v2=texcoord
    AttrInfo_AddLoader(&attrInfo, 3, GPU_FLOAT, 3); // v2=color

    std::vector<vertex_t> vertices = meshVoxels(voxels);

	typedef struct {
		float position[3];
        float normal[3];
		float texcoord[2];
		float color[3];
	} vertex_t_local;

    std::vector<vertex_t_local> vertex_elements_flat;
    vertex_elements_flat.reserve(vertices.size() * U::elements_per_vertex);
    for (auto& vertex : vertices) {
		vertex_elements_flat.emplace_back(vertex_t_local{
			{ vertex[0], vertex[1], vertex[2] },
			{ vertex[3], vertex[4], vertex[5] },
			{ vertex[6], vertex[7] },
            { vertex[8], vertex[9], vertex[10]},
		});
    }

    // Create the VBO (vertex buffer object)
    std::size_t verticesFlatSize = vertex_elements_flat.size() * sizeof(vertex_t_local);

    // todo investigate if this can just be std::vector<float>
    void* vboData = linearAlloc(verticesFlatSize);
    // copy from the vector to the linear memory
    memcpy(vboData, vertex_elements_flat.data(), verticesFlatSize);

    // make vbo, copy vertex data into vbo
    C3D_BufInfo vbo {};
    BufInfo_Init(&vbo);
    BufInfo_Add(&vbo, vboData, 11 * sizeof(float), 4, 0x3210);

    return std::tuple<C3D_AttrInfo, NativeBufferWrapper, std::size_t>(attrInfo, NativeBufferWrapper{ vbo, std::shared_ptr<void>(vboData, [](void* d) { linearFree(d); }) }, vertex_elements_flat.size());
}

void U::NativeRenderer::setClearColor(const RGBColor& color) noexcept
{
	_clearColor = color;
}
