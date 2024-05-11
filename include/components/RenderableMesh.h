#pragma once
#include <util/Constants.h>
#include <components/Renderable.h>

namespace U {

	class RenderableMesh {

	public:

		struct RenderableMeshOptions {
			std::vector<vertex_t> vertices;
			RGBColor color;
		};

		RenderableMesh(const RenderableMeshOptions&& options);

		// void UpdateMesh(const std::vector<vertex_t>& vertices, const RGBColor& color);

		void onRender(entt::registry& registry, entt::entity entity, Renderer& renderer);

	};


}