#pragma once

#include "core/gfx/Canvas.h"

class Plasma {
public:
    Plasma();

    void init();

    void update(float dt);
    void draw(Canvas &canvas);
    void cleanup();

private:
    float _time;
};
