#include "include/components/Square.h"
#include "platform/common/include/Renderer.h"
#include <algorithm>

void Square::render(Renderer& renderer)
{
    auto windowSize = renderer.getWindowSize();

    auto minAxis = std::min(windowSize.height, windowSize.width) * (4 / 5.0);

    Point topLeft;
    topLeft.x = (windowSize.width - minAxis) / 2 + _position.x;
    topLeft.y = (windowSize.height - minAxis) / 2 + _position.y;

    Size size(minAxis, minAxis);

    renderer.drawQuad(topLeft, size, _color);
}