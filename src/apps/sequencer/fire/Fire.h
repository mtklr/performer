#pragma once

#include "core/gfx/Canvas.h"

class Fire {
public:
    Fire();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);
    void cleanup();

private:
    float _time;
};
