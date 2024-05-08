#pragma once
#include "platform/common/include/Renderer.h"
#include "thirdparty/entt/entt.hpp"

/**
 * Some renderable type
*/
struct Renderable : entt::type_list<void(Renderer&)> {
    template <typename Type>
    using impl = entt::value_list<&Type::render>;
};

using renderable
    = entt::poly<Renderable>;
