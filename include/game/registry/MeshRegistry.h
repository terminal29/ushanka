#pragma once
#include <entt/entt.hpp>
#include "util/Constants.h"
#include <platform/Mesh.h>
#include <variant>

namespace U {
	struct NamedMesh {
		static constexpr auto Cube = entt::hashed_string{ "cube" };
	};

	struct MeshLoader final {
		using result_type = std::shared_ptr<Mesh>;

		result_type operator()(result_type mesh) const {
			return mesh;
		}
	};

	auto MeshRegistry = entt::resource_cache<std::shared_ptr<Mesh>, MeshLoader>{};

	void initDefaultMeshes();
}