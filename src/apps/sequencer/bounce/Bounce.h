#pragma once

#include "core/gfx/Canvas.h"

class Bounce {
public:
    Bounce();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);

private:
    float _time;
};
