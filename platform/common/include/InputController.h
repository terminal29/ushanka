#pragma once
#include "include/util/Point.h"

class InputController {

    InputController();

    /**
     * Get the input delta for this frame
    */
    Point frameDelta();
};