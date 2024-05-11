#pragma once
#include <util/Color.h>
#include <glm/vec3.hpp>

namespace U {

    class Renderer;

    struct Square {
        RGBColor _color;
        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);

        Square() = default;
        inline Square(RGBColor color) :_color(color) {};

        void render(Renderer& renderer);
    };

}