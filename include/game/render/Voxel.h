#pragma once
#include "util/Point.h"
#include "util/Color.h"
#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <util/Constants.h>

namespace U {
	struct Voxel {
		glm::ivec3 position;
		RGBColor color;
		entt::hashed_string::hash_type shaderID;
	};

    inline std::vector<U::vertex_t> meshVoxels(const std::vector<U::Voxel>& voxels) {
        std::vector<U::vertex_t> vertices;
		// 2 triangles per face, 3 vertices per triangle, 6 faces per cube
		constexpr static std::size_t per_cube = 2 * 3 * 6;
        vertices.reserve(voxels.size() * per_cube);

        // Cube vertices relative to the center of the voxel
        constexpr static float halfSize = 0.5f;

        // Define normals for each face
        constexpr static std::array<std::array<float, 3>, 6> faceNormals = {
            std::array<float, 3>{1.0f, 0.0f, 0.0f},  // Right
            std::array<float, 3>{-1.0f, 0.0f, 0.0f}, // Left
            std::array<float, 3>{0.0f, 1.0f, 0.0f},  // Top
            std::array<float, 3>{0.0f, -1.0f, 0.0f}, // Bottom
            std::array<float, 3>{0.0f, 0.0f, 1.0f},  // Front
            std::array<float, 3>{0.0f, 0.0f, -1.0f}  // Back
        };

        // Define the 8 vertices of a cube
        constexpr static std::array<std::array<float, 3>, 8> cubeVertices = {
            std::array<float, 3>{-halfSize, -halfSize, -halfSize},
            std::array<float, 3>{halfSize, -halfSize, -halfSize},
            std::array<float, 3>{halfSize, halfSize, -halfSize},
            std::array<float, 3>{-halfSize, halfSize, -halfSize},
            std::array<float, 3>{-halfSize, -halfSize, halfSize},
            std::array<float, 3>{halfSize, -halfSize, halfSize},
            std::array<float, 3>{halfSize, halfSize, halfSize},
            std::array<float, 3>{-halfSize, halfSize, halfSize}
        };

        // Define the 6 faces of the cube, each face has 6 indices for 2 triangles
        constexpr static std::array<std::array<int, 6>, 6> faceIndices = {
            std::array<int, 6>{0, 1, 5, 0, 5, 4}, // Right
            std::array<int, 6>{2, 3, 7, 2, 7, 6}, // Left
            std::array<int, 6>{3, 0, 4, 3, 4, 7}, // Top
            std::array<int, 6>{1, 2, 6, 1, 6, 5}, // Bottom
            std::array<int, 6>{4, 5, 6, 4, 6, 7}, // Front
            std::array<int, 6>{3, 2, 1, 3, 1, 0}  // Back
        };

        for (const auto& voxel : voxels) {
            for (int i = 0; i < 6; ++i) {
                std::array<float, 3> normal = faceNormals[i];

                for (int j = 0; j < 6; ++j) {
                    std::array<float, 3> vertexPos = {
                        voxel.position[0] + cubeVertices[faceIndices[i][j]][0],
                        voxel.position[1] + cubeVertices[faceIndices[i][j]][1],
                        voxel.position[2] + cubeVertices[faceIndices[i][j]][2]
                    };
                    vertices.push_back({ vertexPos[0], vertexPos[1], vertexPos[2], normal[0], normal[1], normal[2], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f});
                }
            }
        }
        return vertices;
    }
}