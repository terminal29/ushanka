#include "components/Tickable.h"
using namespace U;

Tickable::Tickable(std::function<void(float)> onTick)
    : _onTick(onTick ? onTick : [](float deltaTime) {})
{
}

void Tickable::tick(float deltaTime)
{
    // onTick is always valid
    _onTick(deltaTime);
}