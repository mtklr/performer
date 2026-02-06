#pragma once

#include "core/gfx/Canvas.h"

class Stars {
public:
    Stars();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);

private:
    float _time;
};
