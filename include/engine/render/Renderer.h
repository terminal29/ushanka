#pragma once

#include "engine/util/common/Color.h"
#include "engine/util/common/Point.h"
#include "engine/util/common/Size.h"
#include "engine/platform/Platform.h"
#include "engine/components/Camera.h"
#include "engine/components/Transform.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <engine/render/Shader.h>
#include <engine/render/voxel/Voxel.h>
#include <engine/render/RenderObject.h>
#include <engine/util/common/Size.h>

namespace U::Engine::Render {

	namespace Native {
		class NativeRenderer;
	}

	class Renderer {
		std::shared_ptr<U::Engine::Platform::Platform> _platform;

		std::unique_ptr<Native::NativeRenderer> _impl;

	public:
		Renderer(std::shared_ptr<U::Engine::Platform::Platform> platform);

		~Renderer();

		bool waitForVSync() const noexcept;

		const Size getWindowSize() const noexcept;

		void frameBegin() noexcept;

		void frameEnd() noexcept;

		/// <summary>
		/// draws voxels naively from a list of voxels
		/// </summary>
		/// <param name="camera"></param>
		/// <param name="globalOffset"></param>
		/// <param name="voxels"></param>
		/// <returns></returns>
		void drawVoxels(const Camera& camera, const glm::ivec3& globalOffset, const std::vector<U::Engine::Render::Voxel::Voxel>& voxels) noexcept;

		/// <summary>
		/// Draws a voxel object
		/// </summary>
		/// <param name="camera">Camera info</param>
		/// <param name="transform">Global transform of the object</param>
		/// <param name="renderObject">Render object to draw</param>
		/// <returns></returns>
		void draw(const Camera& camera, const Transform& transform, const VoxelRenderObject& renderObject) noexcept;

		void setClearColor(const Color& color) noexcept;

	};

}