#pragma once
#include <entt/entt.hpp>
#include <platform/Renderer.h>

namespace U {

	/**
	* Concept of a Renderable component
	* 
	* Users should implement the onRender method to draw the entity
	*/
	struct Renderable : entt::type_list<void(entt::registry&, entt::entity, Camera& camera, Renderer& )> {
		template<typename Base>
		struct type : Base {
			void onRender(entt::registry& registry, entt::entity entity, Camera& camera, Renderer& renderer ) {
				this->template invoke<0>(*this, registry, entity, camera, renderer);
			}
		};

		template<typename Type>
		using impl = entt::value_list<&Type::onRender>;
	};

	using PolyRenderable = entt::poly<Renderable>;
}