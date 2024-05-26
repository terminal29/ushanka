#include "game/registry/ShaderRegistry.h"
#include "platform/Platform.h"
#include "platform/Renderer.h"
#include "platform/Shader.h"
#include <3ds.h>
#include <NativeRenderer.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <util/Constants.h>
#include <util/Size.h>

constexpr U::Size ThreeDSWindowSize { 400, 240 };

constexpr u32 COMMON_DISPLAY_TRANSFER_FLAGS = (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

// Function to add a face to the vertex data
void addFace(std::vector<U::vertex_t>& vertexData, const std::array<float, 3>& pos, const std::array<float, 3>& normal)
{
    U::vertex_t vertex;

    // Position (3 floats)
    vertex[0] = pos[0];
    vertex[1] = pos[1];
    vertex[2] = pos[2];

    // Normal (3 floats)
    vertex[3] = normal[0];
    vertex[4] = normal[1];
    vertex[5] = normal[2];

    // Texture UV (2 floats)
    vertex[6] = 0.0f; // Example UV coordinate, you might want to calculate or set accordingly
    vertex[7] = 0.0f; // Example UV coordinate, you might want to calculate or set accordingly

    // Color (3 floats, normalized)
    vertex[8] = 1.0f; // Red
    vertex[9] = 1.0f; // Green
    vertex[10] = 1.0f; // Blue

    // Add to vertex data
    vertexData.push_back(vertex);
}

std::vector<U::vertex_t> meshVoxels(const std::vector<U::Voxel>& voxels)
{
    std::vector<U::vertex_t> vertices;

    // Cube vertices relative to the center of the voxel
    const float halfSize = 0.5f;

    // Define normals for each face
    std::array<std::array<float, 3>, 6> faceNormals = {
        std::array<float, 3> { 1.0f, 0.0f, 0.0f }, // Right
        std::array<float, 3> { -1.0f, 0.0f, 0.0f }, // Left
        std::array<float, 3> { 0.0f, 1.0f, 0.0f }, // Top
        std::array<float, 3> { 0.0f, -1.0f, 0.0f }, // Bottom
        std::array<float, 3> { 0.0f, 0.0f, 1.0f }, // Front
        std::array<float, 3> { 0.0f, 0.0f, -1.0f } // Back
    };

    // Define the 8 vertices of a cube
    std::array<std::array<float, 3>, 8> cubeVertices = {
        std::array<float, 3> { -halfSize, -halfSize, -halfSize },
        std::array<float, 3> { halfSize, -halfSize, -halfSize },
        std::array<float, 3> { halfSize, halfSize, -halfSize },
        std::array<float, 3> { -halfSize, halfSize, -halfSize },
        std::array<float, 3> { -halfSize, -halfSize, halfSize },
        std::array<float, 3> { halfSize, -halfSize, halfSize },
        std::array<float, 3> { halfSize, halfSize, halfSize },
        std::array<float, 3> { -halfSize, halfSize, halfSize }
    };

    // Define the 6 faces of the cube, each face has 4 vertices (two triangles)
    std::array<std::array<int, 4>, 6> faceIndices = {
        std::array<int, 4> { 0, 1, 5, 4 }, // Right
        std::array<int, 4> { 2, 3, 7, 6 }, // Left
        std::array<int, 4> { 3, 0, 4, 7 }, // Top
        std::array<int, 4> { 1, 2, 6, 5 }, // Bottom
        std::array<int, 4> { 4, 5, 6, 7 }, // Front
        std::array<int, 4> { 3, 2, 1, 0 } // Back
    };

    for (const auto& voxel : voxels) {
        for (int i = 0; i < 6; ++i) {
            std::array<float, 3> normal = faceNormals[i];

            for (int j = 0; j < 4; ++j) {
                std::array<float, 3> vertexPos = {
                    voxel.position[0] + cubeVertices[faceIndices[i][j]][0],
                    voxel.position[1] + cubeVertices[faceIndices[i][j]][1],
                    voxel.position[2] + cubeVertices[faceIndices[i][j]][2]
                };
                addFace(vertices, vertexPos, normal);
            }
        }
    }
    return vertices;
}

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
    unsigned long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    float hue = (milliseconds_since_epoch / 100) % 360 / 360.0f;
    auto color = HSLToRGB(hue, 1.0, 0.5);
    u32 colorAsInt = _createRGBA(color.r, color.g, color.b, 255);
    C3D_RenderTargetClear(_rtTopLeft.get(), C3D_CLEAR_ALL, colorAsInt, 0);
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_FrameDrawOn(_rtTopLeft.get());
}

void U::NativeRenderer::frameEnd()
{
    printf("Ushanka game by jaocb1\n");
    printf("CPU:     %6.2f%%\n", C3D_GetProcessingTime() * 6.0f);
    printf("GPU:     %6.2f%%\n", C3D_GetDrawingTime() * 6.0f);
    printf("CmdBuf:  %6.2f%%\n", C3D_GetCmdBufUsage() * 100.0f);
    std::cout.flush();
    std::cout << "\x1B[H";
    C3D_FrameEnd(0);
}

bool U::NativeRenderer::waitForVSync() const noexcept
{
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
    const glm::fmat4 model = glm::translate(glm::fmat4(1), glm::vec3(globalOffset));

    // set shader uniforms
    const glm::fmat4 view = camera.getViewMatrix();
    const glm::fmat4 projection = camera.getProjectionMatrix();

    auto [vao, vbo, vertexCount] = makeVoxelVBOAttrs(voxels);
    // bind ~vao
    C3D_SetAttrInfo(&vao);
    // bind ~vbo
    C3D_SetBufInfo(&vbo);
    std::cout << "rendering " << (vertexCount / elements_per_vertex) << "vertices (from {" << voxels.size() << "} voxels)"
              << "\n";

    shader->setUniformMat4(Shader::ENamedShaderUniform::ModelMatrix, model);
    shader->setUniformMat4(Shader::ENamedShaderUniform::ViewMatrix, view);
    shader->setUniformMat4(Shader::ENamedShaderUniform::ProjectionMatrix, projection);
    shader->setUniformMat3(Shader::ENamedShaderUniform::NormalMatrix, glm::fmat3(glm::transpose(glm::inverse(view * model))));
    shader->setUniformVec3(Shader::ENamedShaderUniform::AmbientLightColor, glm::fvec3(0.5, 0.5, 0.5));
    shader->setUniformVec3(Shader::ENamedShaderUniform::SunDirection, glm::fvec3(0, 1, 0));
    shader->setUniformVec3(Shader::ENamedShaderUniform::SunColor, glm::fvec3(0.5, 0.5, 0.5));

    C3D_DrawArrays(GPU_TRIANGLES, 0, vertexCount);
    shader->unbind();
}

std::tuple<C3D_AttrInfo, C3D_BufInfo, std::size_t> U::NativeRenderer::makeVoxelVBOAttrs(const std::vector<U::Voxel>& voxels) noexcept
{
    // Make attribute info (but dont bind it yet)
    constexpr std::size_t vertex_stride = 3;
    constexpr std::size_t normal_stride = 3;
    constexpr std::size_t texture_coord_stride = 2;
    constexpr std::size_t color_stride = 3;
    C3D_AttrInfo attrInfo;
    AttrInfo_Init(&attrInfo);
    AttrInfo_AddLoader(&attrInfo, 0, GPU_FLOAT, vertex_stride); // v0=position
    AttrInfo_AddLoader(&attrInfo, 1, GPU_FLOAT, normal_stride); // v1=normal
    AttrInfo_AddLoader(&attrInfo, 2, GPU_FLOAT, texture_coord_stride); // v2=texcoord
    AttrInfo_AddLoader(&attrInfo, 3, GPU_FLOAT, color_stride); // v2=color

    std::vector<vertex_t> vertices = meshVoxels(voxels);

    std::vector<U::vertex_element_t> vertex_elements_flat;
    vertex_elements_flat.reserve(vertices.size() * U::elements_per_vertex);
    for (auto& vertex : vertices) {
        for (auto& element : vertex) {
            vertex_elements_flat.push_back(element);
        }
    }

    // Create the VBO (vertex buffer object)

    // todo investigate if this can just be std::vector<float>
    auto vboData = std::shared_ptr<void>(linearAlloc(vertices.size() * elements_per_vertex * sizeof(vertex_element_t)), [](void* vbo) {
        linearFree(vbo);
    });
    // copy from the vector to the linear memory
    memcpy(vboData.get(), vertex_elements_flat.data(), vertex_elements_flat.size() * sizeof(vertex_element_t));

    // make vbo, copy vertex data into vbo
    C3D_BufInfo vbo {};
    BufInfo_Init(&vbo);
    BufInfo_Add(&vbo, vboData.get(), sizeof(vertex_element_t), 4, 0x3210);

    return std::tuple<C3D_AttrInfo, C3D_BufInfo, std::size_t>(attrInfo, vbo, vertex_elements_flat.size());
}
