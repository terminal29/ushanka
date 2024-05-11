#pragma once
#include <array>

namespace U {

    using vertex_element_t = float;
    
    static constexpr int per_vertex = 3;

    using vertex_t = std::array<vertex_element_t, per_vertex>;

    // just some arbitrary number until more testing done on 3ds
    static constexpr std::size_t maxVertices = 200;
}