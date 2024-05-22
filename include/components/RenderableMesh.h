#pragma once
#include <util/Constants.h>
#include <components/Renderable.h>
#include <platform/Mesh.h>
#include <components/Camera.h>

/*
namespace U {

	class RenderableMesh {

	public:

		std::shared_ptr<Mesh> _mesh;
		std::shared_ptr<Shader> _shader;

		struct RenderableMeshOptions {
			explicit inline RenderableMeshOptions(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, RGBColor color) : mesh(mesh), shader(shader), color(color) {}
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Shader> shader;
			RGBColor color;
		};

		RenderableMesh(const RenderableMeshOptions&& options);

		void onRender(entt::registry& registry, entt::entity entity, Camera& camera, Renderer& renderer);

		~RenderableMesh();

	};


}
*/