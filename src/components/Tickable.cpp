#include "include/components/Tickable.h"

Tickable::Tickable(std::function<void(float)> onTick)
    : _onTick(onTick ? onTick : [](float deltaTime) {})
{
}

void Tickable::tick(float deltaTime)
{
    // onTick is always valid
    _onTick(deltaTime);
}